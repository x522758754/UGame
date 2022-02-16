//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Misc/Guid.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Animation/AnimClassInterface.h"

#include "Pixel2DTDAnimBlueprintGeneratedClass.generated.h"

class UPixel2DTDAnimGraphNode_Base;
class UPixel2DTDAnimGraphNode_StateMachineBase;

// This structure represents debugging information for a single state machine
USTRUCT()
struct FPixel2DTDStateMachineDebugData
{
	GENERATED_USTRUCT_BODY()

public:
	// Map from state nodes to their state entry in a state machine
	TMap<TWeakObjectPtr<UEdGraphNode>, int32> NodeToStateIndex;
	TMap<TWeakObjectPtr<UEdGraphNode>, int32> NodeToTransitionIndex;

	// The animation node that leads into this state machine (A3 only)
	TWeakObjectPtr<UPixel2DTDAnimGraphNode_StateMachineBase> MachineInstanceNode;

	// Index of this machine in the StateMachines array
	int32 MachineIndex;
};

// This structure represents animation-related debugging information for an entire PixelAnimBlueprint
// (general debug information for the event graph, etc... is still contained in a FBlueprintDebugData structure)
USTRUCT()
struct PIXEL2DTD_API FPixel2DTDAnimBlueprintDebugData
{
	GENERATED_USTRUCT_BODY()

		FPixel2DTDAnimBlueprintDebugData()
	{
	}

#if WITH_EDITORONLY_DATA
public:
	// Map from state machine graphs to their corresponding debug data
	TMap<TWeakObjectPtr<const UEdGraph>, FPixel2DTDStateMachineDebugData> StateMachineDebugData;
#endif
};

UCLASS()
class PIXEL2DTD_API UPixel2DTDAnimBlueprintGeneratedClass : public UBlueprintGeneratedClass//, public IAnimClassInterface
{
	GENERATED_UCLASS_BODY()

	// List of state machines present in this blueprint class
	UPROPERTY()
	TArray<FBakedAnimationStateMachine> BakedStateMachines;

	// The index of the root node in the animation tree
	UPROPERTY()
	int32 RootAnimNodeIndex;

	// The array of anim nodes; this is transient generated data (created during Link)
	FStructProperty* RootAnimNodeProperty;
	TArray<FStructProperty*> AnimNodeProperties;

	// Map from animation node GUID to property index
	TMap<FGuid, int32> NodeGuidToIndexMap;
public:

	virtual const TArray<FBakedAnimationStateMachine>& GetBakedStateMachines() const /*override*/ { return BakedStateMachines; }

	virtual int32 GetRootAnimNodeIndex() const /*override*/ { return RootAnimNodeIndex; }

	virtual FStructProperty* GetRootAnimNodeProperty() const /*override*/ { return RootAnimNodeProperty; }

	virtual const TArray<FStructProperty*>& GetAnimNodeProperties() const /*override*/ { return AnimNodeProperties; }

public:
#if WITH_EDITORONLY_DATA
	FPixel2DTDAnimBlueprintDebugData AnimBlueprintDebugData;

	FPixel2DTDAnimBlueprintDebugData& GetAnimBlueprintDebugData()
	{
		return AnimBlueprintDebugData;
	}
#endif

	// UStruct interface
	virtual void Link(FArchive& Ar, bool bRelinkExistingProperties) override;
	// End of UStruct interface

    // UClass interface
	virtual void PurgeClass(bool bRecompilingOnLoad) override;
	virtual void PostLoadDefaultObject(UObject* Object) override;
	// End of UClass interface
};

template<typename NodeType>
NodeType* GetSpriteNodeFromPropertyIndex(UObject* AnimInstanceObject, const UPixel2DTDAnimBlueprintGeneratedClass* AnimBlueprintClass, int32 PropertyIndex)
{
	if (PropertyIndex != INDEX_NONE)
	{
		FStructProperty* NodeProperty = AnimBlueprintClass->GetAnimNodeProperties()[AnimBlueprintClass->GetAnimNodeProperties().Num() - 1 - PropertyIndex]; //REVISIT
		check(NodeProperty->Struct == NodeType::StaticStruct());
		return NodeProperty->ContainerPtrToValuePtr<NodeType>(AnimInstanceObject);
	}

	return NULL;
}
