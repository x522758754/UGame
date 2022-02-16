//Polyart Studio 2021
 

#include "Pixel2DTDAnimInstanceProxy.h"
#include "Pixel2DTDAnimNode_Base.h"
#include "Pixel2DTDAnimNode_StateMachine.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimInstance"

void FPixel2DTDAnimInstanceProxy::UpdateAnimationNode(float DeltaSeconds)
{
	if (RootNode != nullptr)
	{
		UpdateCounter.Increment();
		RootNode->Update_AnyThread(FPixel2DTDAnimationUpdateContext(this, DeltaSeconds, &NotifyQueue));
	}
}

void FPixel2DTDAnimInstanceProxy::Initialize(UPixel2DTDAnimInstance* InAnimInstance)
{
	// copy anim instance object if it has not already been set up
	AnimInstanceObject = InAnimInstance;

	AnimClassInterface = Cast<UPixel2DTDAnimBlueprintGeneratedClass>(InAnimInstance->GetClass());

	if (UPixel2DTDAnimBlueprintGeneratedClass* AnimBlueprintClass = Cast<UPixel2DTDAnimBlueprintGeneratedClass>(InAnimInstance->GetClass()))
	{
		// Grab a pointer to the root node
		if (FStructProperty* RootAnimNodeProperty = AnimBlueprintClass->GetRootAnimNodeProperty())
		{
			RootNode = RootAnimNodeProperty->ContainerPtrToValuePtr<FPixel2DTDAnimNode_Base>(InAnimInstance);
		}
		else
		{
			RootNode = nullptr;
		}
	}
}

void FPixel2DTDAnimInstanceProxy::InitializeRootNode()
{
	if (RootNode != nullptr)
	{
		auto InitializeNode = [this](FPixel2DTDAnimNode_Base* AnimNode)
		{
			// Force our functions to be re-evaluated - this reinitialization may have been a 
			// consequence of our class being recompiled and functions will be invalid in that
			// case.
			AnimNode->EvaluateGraphExposedInputs.bInitialized = false;
			AnimNode->EvaluateGraphExposedInputs.Initialize(AnimNode, AnimInstanceObject);
		};

		if (AnimClassInterface)
		{
			// cache any state machine descriptions we have
			for (FStructProperty* Property : AnimClassInterface->GetAnimNodeProperties())
			{
				if (Property->Struct->IsChildOf(FPixel2DTDAnimNode_Base::StaticStruct()))
				{
					FPixel2DTDAnimNode_Base* AnimNode = Property->ContainerPtrToValuePtr<FPixel2DTDAnimNode_Base>(AnimInstanceObject);
					if (AnimNode)
					{
						InitializeNode(AnimNode);

						if (Property->Struct->IsChildOf(FPixel2DTDAnimNode_StateMachine::StaticStruct()))
						{
							FPixel2DTDAnimNode_StateMachine* StateMachine = static_cast<FPixel2DTDAnimNode_StateMachine*>(AnimNode);
							StateMachine->CacheMachineDescription(AnimClassInterface);
						}
					}
				}
			}
		}

		FPixel2DTDAnimationInitializeContext InitContext(this);
		RootNode->Initialize_AnyThread(InitContext);
	}
}

void FPixel2DTDAnimInstanceProxy::Uninitialize(UPixel2DTDAnimInstance* InAnimInstance)
{
}

void FPixel2DTDAnimInstanceProxy::PreUpdate(UPixel2DTDAnimInstance* InAnimInstance, float DeltaSeconds)
{
	CurrentDeltaSeconds = DeltaSeconds;

	NotifyQueue.Reset();
}

void FPixel2DTDAnimInstanceProxy::PostUpdate(UPixel2DTDAnimInstance* InAnimInstance) const
{
	InAnimInstance->NotifyQueue.Append(NotifyQueue);
}

void FPixel2DTDAnimInstanceProxy::TickAssetPlayerInstances(float DeltaSeconds)
{
}

FPixel2DTDAnimNode_Base* FPixel2DTDAnimInstanceProxy::GetNodeFromIndexUntyped(int32 NodeIdx, UScriptStruct* RequiredStructType)
{
	FPixel2DTDAnimNode_Base* NodePtr = nullptr;
	if (AnimClassInterface)
	{
		const TArray<FStructProperty*>& AnimNodeProperties = AnimClassInterface->GetAnimNodeProperties();
		const int32 InstanceIdx = AnimNodeProperties.Num() - 1 - NodeIdx;

		if (AnimNodeProperties.IsValidIndex(InstanceIdx))
		{
			FStructProperty* NodeProperty = AnimNodeProperties[InstanceIdx];

			if (NodeProperty->Struct->IsChildOf(RequiredStructType))
			{
				NodePtr = NodeProperty->ContainerPtrToValuePtr<FPixel2DTDAnimNode_Base>(AnimInstanceObject);
			}
		}
	}

	return NodePtr;
}

void FPixel2DTDAnimInstanceProxy::UpdateAnimation()
{
	// update native update
	{
		Update(CurrentDeltaSeconds);
	}

	// update all nodes
	UpdateAnimationNode(CurrentDeltaSeconds);

	// tick all our active asset players
	TickAssetPlayerInstances(CurrentDeltaSeconds);
}

void FPixel2DTDAnimInstanceProxy::EvaluateAnimation(FPixel2DTDPoseContext& Output)
{
	EvaluateAnimationNode(Output);
}

void FPixel2DTDAnimInstanceProxy::EvaluateAnimationNode(FPixel2DTDPoseContext& Output)
{
	if (RootNode != NULL)
	{
		RootNode->Evaluate_AnyThread(Output);
	}
}

float FPixel2DTDAnimInstanceProxy::GetInstanceCurrentStateElapsedTime(int32 MachineIndex)
{
	if (FPixel2DTDAnimNode_StateMachine* MachineInstance = GetStateMachineInstance(MachineIndex))
	{
		return MachineInstance->GetCurrentStateElapsedTime();
	}

	return 0.0f;
}

float FPixel2DTDAnimInstanceProxy::GetRelevantAnimTimeRemaining(int32 MachineIndex, int32 StateIndex)
{
	if (FPixel2DTDAnimNode_AssetSprite* AssetPlayer = GetRelevantAssetPlayerFromState(MachineIndex, StateIndex))
	{
		if (AssetPlayer->GetAnimAsset())
		{
			return AssetPlayer->GetCurrentAssetLength() - GetInstanceCurrentStateElapsedTime(MachineIndex);
		}
	}

	return MAX_flt;
}

float FPixel2DTDAnimInstanceProxy::GetRelevantAnimTimeRemainingFraction(int32 MachineIndex, int32 StateIndex)
{
	if (FPixel2DTDAnimNode_AssetSprite* AssetPlayer = GetRelevantAssetPlayerFromState(MachineIndex, StateIndex))
	{
		if (AssetPlayer->GetAnimAsset())
		{
			float Length = AssetPlayer->GetCurrentAssetLength();
			if (Length > 0.0f)
			{
				return (Length - GetInstanceCurrentStateElapsedTime(MachineIndex)) / Length;
			}
		}
	}

	return 1.0f;
}

float FPixel2DTDAnimInstanceProxy::GetRelevantAnimLength(int32 MachineIndex, int32 StateIndex)
{
	if (FPixel2DTDAnimNode_AssetSprite* AssetPlayer = GetRelevantAssetPlayerFromState(MachineIndex, StateIndex))
	{
		if (AssetPlayer->GetAnimAsset())
		{
			return AssetPlayer->GetCurrentAssetLength();
		}
	}

	return 0.0f;
}

float FPixel2DTDAnimInstanceProxy::GetRelevantAnimTime(int32 MachineIndex, int32 StateIndex)
{
	return GetInstanceCurrentStateElapsedTime(MachineIndex);
}

float FPixel2DTDAnimInstanceProxy::GetRelevantAnimTimeFraction(int32 MachineIndex, int32 StateIndex)
{
	if (FPixel2DTDAnimNode_AssetSprite* AssetPlayer = GetRelevantAssetPlayerFromState(MachineIndex, StateIndex))
	{
		float Length = AssetPlayer->GetCurrentAssetLength();
		if (Length > 0.0f)
		{
			return GetInstanceCurrentStateElapsedTime(MachineIndex) / Length;
		}
	}

	return 0.0f;
}

FPixel2DTDAnimNode_AssetSprite* FPixel2DTDAnimInstanceProxy::GetRelevantAssetPlayerFromState(int32 MachineIndex, int32 StateIndex)
{
	FPixel2DTDAnimNode_AssetSprite* ResultPlayer = nullptr;
	if (FPixel2DTDAnimNode_StateMachine* MachineInstance = GetStateMachineInstance(MachineIndex))
	{
		float MaxWeight = 0.0f;
		const FBakedAnimationState& State = MachineInstance->GetStateInfo(StateIndex);
		for (const int32& PlayerIdx : State.PlayerNodeIndices)
		{
			if (FPixel2DTDAnimNode_AssetSprite* Player = GetNodeFromIndex<FPixel2DTDAnimNode_AssetSprite>(PlayerIdx))
			{
				ResultPlayer = Player;
			}
		}
	}
	return ResultPlayer;
}

FPixel2DTDAnimNode_StateMachine* FPixel2DTDAnimInstanceProxy::GetStateMachineInstance(int32 MachineIndex)
{
	if (AnimClassInterface)
	{
		const TArray<FStructProperty*>& AnimNodeProperties = AnimClassInterface->GetAnimNodeProperties();
		if ((MachineIndex >= 0) && (MachineIndex < AnimNodeProperties.Num()))
		{
			const int32 InstancePropertyIndex = AnimNodeProperties.Num() - 1 - MachineIndex;

			FStructProperty* MachineInstanceProperty = AnimNodeProperties[InstancePropertyIndex];
			checkSlow(MachineInstanceProperty->Struct->IsChildOf(FPixel2DTDAnimNode_StateMachine::StaticStruct()));

			return MachineInstanceProperty->ContainerPtrToValuePtr<FPixel2DTDAnimNode_StateMachine>(AnimInstanceObject);
		}
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
