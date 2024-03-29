//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node_CallFunction.h"
#include "BlueprintActionFilter.h"
#include "UK2Node_FlipbookGetterTD.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UPixel2DTDAnimBlueprint;
class UPixel2DTDAnimGraphNode_Base;
class UPixel2DTDAnimStateNodeBase;

USTRUCT()
struct FPixel2DTDNodeSpawnData
{
	GENERATED_BODY()

	FPixel2DTDNodeSpawnData();

	// Title to use for the spawned node
	UPROPERTY()
	FText CachedTitle;

	// The node the spawned getter accesses
	UPROPERTY()
	UPixel2DTDAnimGraphNode_Base* SourceNode;

	// The state node the spawned getter accesses
	UPROPERTY()
	UPixel2DTDAnimStateNodeBase* SourceStateNode;

	// The instance class the spawned getter is defined on
	UPROPERTY()
	UClass* AnimInstanceClass;

	// The blueprint the getter is valid within
	UPROPERTY()
	const UPixel2DTDAnimBlueprint* SourceBlueprint;

	// The UFunction (as a UField) 
	UPROPERTY()
	UField* Getter;

	// String of combined valid contexts for the spawned getter
	UPROPERTY()
	FString GetterContextString;
};

UCLASS(MinimalAPI)
class UK2Node_FlipbookGetterTD : public UK2Node_CallFunction
{
	GENERATED_BODY()
public:

	// UObject interface
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostPasteNode() override;
	// End of UObject interface

	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual bool IsActionFilteredOut(FBlueprintActionFilter const& Filter) override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// end of UK2Node interface

	// The node that is required for the getter
	UPROPERTY()
	UPixel2DTDAnimGraphNode_Base* SourceNode;

	// UPixel2DTDAnimStateNode doesn't use the same hierarchy so we need to have a seperate property here to handle
	// those.
	UPROPERTY()
	UPixel2DTDAnimStateNodeBase* SourceStateNode;

	// The UAnimInstance derived class that implements the getter we are running
	UPROPERTY()
	UClass* GetterClass;

	// The anim blueprint that generated this getter
	UPROPERTY()
	const UPixel2DTDAnimBlueprint* SourceAnimBlueprint;

	// Cached node title
	UPROPERTY()
	FText CachedTitle;

	// List of valid contexts for the node
	UPROPERTY()
	TArray<FString> Contexts;

protected:
	// Fixes the SourceNode to be the state machine owner of SourceStateNode (if it is not null)
	void RestoreStateMachineNode();

	/** Returns whether or not the provided UFunction requires the named parameter */
	bool GetterRequiresParameter(const UFunction* Getter, FString ParamName) const;

	/** Checks the cached context strings to make sure this getter is valid within the provided schema */
	bool IsContextValidForSchema(const UEdGraphSchema* Schema) const;

	/** Passed to blueprint spawners to configure spawned nodes */
	void PostSpawnNodeSetup(UEdGraphNode* NewNode, bool bIsTemplateNode, FPixel2DTDNodeSpawnData SpawnData);
};
