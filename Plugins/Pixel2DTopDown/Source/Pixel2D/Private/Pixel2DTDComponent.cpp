//Polyart Studio 2021
 

#include "Pixel2DTDComponent.h"
#include "Pixel2DTDAnimInstance.h"

UPixel2DTDComponent::UPixel2DTDComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GlobalAnimRateScale = 1.0f;
	bAdjustBounds = false; // Override parent
}

#if WITH_EDITOR
void UPixel2DTDComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitAnim();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UPixel2DTDComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitAnim();
}

void UPixel2DTDComponent::OnRegister()
{
	Super::OnRegister();

	// We force an initialization here because we're in one of two cases.
	// 1) First register, no spawned instance, need to initialize
	// 2) We're being re-registered, in which case we've went through
	// OnUnregister and unconditionally uninitialized our anim instances
	// so we need to force initialize them before we begin to tick.
	InitAnim();

	if (!FApp::CanEverRender())
	{
		SetComponentTickEnabled(false);
	}
}

void UPixel2DTDComponent::SetAnimInstance(UClass * NewAnimInstance)
{
	if (NewAnimInstance != SpriteAnimInstance)
	{
		AActor* ComponentOwner = GetOwner();
		
		if ((ComponentOwner == nullptr) || AreDynamicDataChangesAllowed())
		{
			SpriteAnimInstance = NewAnimInstance;
			InitAnim(/*true*/);
		}
	}
}

bool UPixel2DTDComponent::InitializeAnimScriptInstance()
{
	AnimScriptInstance = NewObject<UPixel2DTDAnimInstance>(this, SpriteAnimInstance);
	if (AnimScriptInstance)
	{
		AnimScriptInstance->InitializeAnimation();
	}
	return true;
}

bool UPixel2DTDComponent::NeedToSpawnAnimScriptInstance() const
{
	if (SpriteAnimInstance)
	{
		return true;
	}
	return false;
}

void UPixel2DTDComponent::InitAnim() 
{
	if (NeedToSpawnAnimScriptInstance())
	{
		const bool bInitializedAnimInstance = InitializeAnimScriptInstance();
		TickAnimation(0, false);
	}
}

bool UPixel2DTDComponent::ShouldTickAnimation() const
{
	return true;
}

void UPixel2DTDComponent::TickAnimation(float DeltaTime, bool bNeedsValidRootMotion)
{
	if (GetFlipbook() || !bNeedsValidRootMotion)
	{
		if (AnimScriptInstance != nullptr)
		{
			// Tick the animation
			AnimScriptInstance->UpdateAnimation(DeltaTime * GlobalAnimRateScale, bNeedsValidRootMotion);
			UPaperFlipbook * CurrentFlipbook = NULL;
			UPaperFlipbook * CurrentFlipbook1 = EvaluateAnimation(this, AnimScriptInstance, CurrentFlipbook);

			if (CurrentFlipbook1 != SourceFlipbook)
			{
				SetFlipbook(CurrentFlipbook1);
			}

			// Dispatch queued events
			AnimScriptInstance->DispatchQueuedAnimEvents();
		}
	}
}

void UPixel2DTDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TickAnimation(DeltaTime, false);
}

bool UPixel2DTDComponent::HandleExistingParallelEvaluationTask(bool bBlockOnTask, bool bPerformPostAnimEvaluation)
{
	return false;
}

UPaperFlipbook * UPixel2DTDComponent::EvaluateAnimation(const UPixel2DTDComponent* InSkeletalMesh, UPixel2DTDAnimInstance* InAnimInstance, UPaperFlipbook * OutPose) const
{
	return InAnimInstance->ParallelEvaluateAnimation(InSkeletalMesh, OutPose);
}
