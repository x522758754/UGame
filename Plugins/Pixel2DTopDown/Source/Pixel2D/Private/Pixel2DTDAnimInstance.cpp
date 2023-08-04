//Polyart Studio 2021
 

#include "Pixel2DTDAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Pixel2DTDAnimInstanceProxy.h"

#include "Pixel2DTDAnimNode_Base.h"

UPixel2DTDAnimInstance::UPixel2DTDAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bUpdatingAnimation(false)
	, bPostUpdatingAnimation(false)
{
	bNeedsUpdate = false;

	// Default to using threaded animation update.
	bUseMultiThreadedAnimationUpdate = true;
}

APawn* UPixel2DTDAnimInstance::TryGetPawnOwner() const
{
	UPixel2DTDComponent* OwnerComponent = GetPaperSpriteComponent();
	if (AActor* OwnerActor = OwnerComponent->GetOwner())
	{
		return Cast<APawn>(OwnerActor);
	}

	return NULL;
}

#define LOCTEXT_NAMESPACE "SpriteAnimInstance"

AActor* UPixel2DTDAnimInstance::GetOwningActor() const
{
	UPixel2DTDComponent* OwnerComponent = GetPaperSpriteComponent();
	return OwnerComponent->GetOwner();
}

UPixel2DTDComponent* UPixel2DTDAnimInstance::GetOwningComponent() const
{
	return GetSpriteComponent();
}

void UPixel2DTDAnimInstance::InitializeAnimation()
{
	UninitializeAnimation();

	UPixel2DTDComponent* OwnerComponent = GetSpriteComponent();
	if (OwnerComponent->GetFlipbook() != NULL)
	{
		CurrentFlipbook = OwnerComponent->GetFlipbook();
	}
	else
	{
		CurrentFlipbook = NULL;
	}

	GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>().Initialize(this);
	GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>().InitializeRootNode();
}

void UPixel2DTDAnimInstance::UninitializeAnimation()
{
    GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>().Uninitialize(this);
	NotifyQueue.Reset();
}

void UPixel2DTDAnimInstance::UpdateAnimation(float DeltaSeconds, bool bNeedsValidRootMotion)
{
#if DO_CHECK
	checkf(!bUpdatingAnimation, TEXT("UpdateAnimation already in progress, circular detected for Flipbook [%s], AnimInstance [%s]"), *GetNameSafe(GetOwningComponent()), *GetName());
	TGuardValue<bool> CircularGuard(bUpdatingAnimation, true);
#endif

	// acquire the proxy as we need to update
	FPixel2DTDAnimInstanceProxy& Proxy = GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>();
	NotifyQueue.Reset();

	PreUpdateAnimation(DeltaSeconds);
	BlueprintUpdateAnimation(DeltaSeconds);

	if (bNeedsValidRootMotion || NeedsImmediateUpdate(DeltaSeconds))
	{
		// cant use parallel update, so just do the work here
		Proxy.UpdateAnimation();
		PostUpdateAnimation();
	}
}

void UPixel2DTDAnimInstance::PreUpdateAnimation(float DeltaSeconds)
{
	bNeedsUpdate = true;
	NotifyQueue.Reset();

	GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>().PreUpdate(this, DeltaSeconds);
}

void UPixel2DTDAnimInstance::PostUpdateAnimation()
{
	bNeedsUpdate = false;

	// acquire the proxy as we need to update
	FPixel2DTDAnimInstanceProxy& Proxy = GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>();

	Proxy.PostUpdate(this);
}

void UPixel2DTDAnimInstance::DispatchQueuedAnimEvents()
{
	// now trigger Notifies
	TriggerAnimNotifies(GetProxyOnGameThread<FPixel2DTDAnimInstanceProxy>().GetDeltaSeconds());
}

bool UPixel2DTDAnimInstance::NeedsImmediateUpdate(float DeltaSeconds) const
{
	return true;
}

bool UPixel2DTDAnimInstance::NeedsUpdate() const
{
	return bNeedsUpdate;
}

bool UPixel2DTDAnimInstance::ParallelCanEvaluate(const UPixel2DTDComponent* InSkeletalMesh) const
{
	return true;
}

UPaperFlipbook* UPixel2DTDAnimInstance::ParallelEvaluateAnimation(const UPixel2DTDComponent* InSkeletalMesh, UPaperFlipbook * Flipbook)
{
	FPixel2DTDAnimInstanceProxy& Proxy = GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>();

	FMemMark Mark(FMemStack::Get());

	FPixel2DTDPoseContext EvaluationContext(&Proxy);

	// Run the anim blueprint
	Proxy.EvaluateAnimation(EvaluationContext);
	Flipbook = EvaluationContext.Flipbook;

	return Flipbook;
}

bool UPixel2DTDAnimInstance::CanTransitionSignature() const
{
	return false;
}

void UPixel2DTDAnimInstance::PostInitProperties()
{
	Super::PostInitProperties();

	if (AnimInstanceProxy == nullptr)
	{
		AnimInstanceProxy = CreateAnimInstanceProxy();
		check(AnimInstanceProxy != nullptr);
	}
}

void UPixel2DTDAnimInstance::TriggerAnimNotifies(float DeltaSeconds)
{
	for (int32 Index = 0; Index<NotifyQueue.AnimNotifies.Num(); Index++)
	{
		const FAnimNotifyEvent* AnimNotifyEvent = NotifyQueue.AnimNotifies[Index].GetNotify();

		if (!AnimNotifyEvent)
			continue;

		// AnimNotifyState
		if (AnimNotifyEvent->NotifyStateClass)
		{
			continue;
		}

		// Trigger non 'state' AnimNotifies
		TriggerSingleAnimNotify(AnimNotifyEvent);
	}
}

void UPixel2DTDAnimInstance::TriggerSingleAnimNotify(const FAnimNotifyEvent* AnimNotifyEvent)
{
	// This is for non 'state' anim notifies.
	if (AnimNotifyEvent && (AnimNotifyEvent->NotifyStateClass == NULL))
	{
		if (AnimNotifyEvent->Notify != NULL)
		{
		}
		else if (AnimNotifyEvent->NotifyName != NAME_None)
		{
			// Custom Event based notifies. These will call a AnimNotify_* function on the AnimInstance.
			UFunction* Function = FindFunction(AnimNotifyEvent->NotifyName);
			if (Function)
			{
				// if parameter is none, add event
				if (Function->NumParms == 0)
				{
					ProcessEvent(Function, NULL);
				}
				else
				{
					// Actor has event, but with different parameters. Print warning
					//UE_LOG(LogSpriteAnimEditor, Warning, TEXT("Pixel 2DAnim notifier named %s, but the parameter number does not match or not of the correct type"), *FuncName);
				}
			}

			 if(FPixel2DTDAnimNotifyEvent* NotifyEvent = (FPixel2DTDAnimNotifyEvent*)AnimNotifyEvent)
			 {
			 	if(NotifyEvent->EventTag.IsValid())
			 	{
			 		if(AActor* Owner = GetOwningActor())
			 		{
			 			UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
			 			if (AbilitySystemComponent != nullptr)
			 			{
			 				FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			 				AbilitySystemComponent->HandleGameplayEvent(NotifyEvent->EventTag, new FGameplayEventData());
			 			}
			 		}
			 	}
			}
		}
	}
}
 
float UPixel2DTDAnimInstance::GetInstanceCurrentStateElapsedTime(int32 MachineIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetInstanceCurrentStateElapsedTime(MachineIndex);
}

float UPixel2DTDAnimInstance::GetRelevantAnimTimeRemaining(int32 MachineIndex, int32 StateIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetRelevantAnimTimeRemaining(MachineIndex, StateIndex);
}

float UPixel2DTDAnimInstance::GetRelevantAnimTimeRemainingFraction(int32 MachineIndex, int32 StateIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetRelevantAnimTimeRemainingFraction(MachineIndex, StateIndex);
}

float UPixel2DTDAnimInstance::GetRelevantAnimLength(int32 MachineIndex, int32 StateIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetRelevantAnimLength(MachineIndex, StateIndex);
}

float UPixel2DTDAnimInstance::GetRelevantAnimTime(int32 MachineIndex, int32 StateIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetRelevantAnimTime(MachineIndex, StateIndex);
}

float UPixel2DTDAnimInstance::GetRelevantAnimTimeFraction(int32 MachineIndex, int32 StateIndex)
{
	return GetProxyOnAnyThread<FPixel2DTDAnimInstanceProxy>().GetRelevantAnimTimeFraction(MachineIndex, StateIndex);
}

bool UPixel2DTDAnimInstance::IsRunningParallelEvaluation() const
{
	return false;
}
//
FPixel2DTDAnimInstanceProxy* UPixel2DTDAnimInstance::CreateAnimInstanceProxy()
{
	return new FPixel2DTDAnimInstanceProxy(this);
}

void UPixel2DTDAnimInstance::DestroyAnimInstanceProxy(FPixel2DTDAnimInstanceProxy* InProxy)
{
	delete InProxy;
}

void UPixel2DTDAnimInstance::PreInitializeRootNode()
{
	// This function should only be called on the CDO
	check(HasAnyFlags(RF_ClassDefaultObject));

	UPixel2DTDAnimBlueprintGeneratedClass* AnimClassInterface = Cast<UPixel2DTDAnimBlueprintGeneratedClass>(GetClass());

	if (AnimClassInterface)
	{
		for (FStructProperty* Property : AnimClassInterface->GetAnimNodeProperties())
		{
			if (Property->Struct->IsChildOf(FPixel2DTDAnimNode_Base::StaticStruct()))
			{
				FPixel2DTDAnimNode_Base* AnimNode = Property->ContainerPtrToValuePtr<FPixel2DTDAnimNode_Base>(this);
				if (AnimNode)
				{
					AnimNode->EvaluateGraphExposedInputs.Initialize(AnimNode, this);
				}
			}
		}
	}
}
#undef LOCTEXT_NAMESPACE 
