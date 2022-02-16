//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "Pixel2DTDAnimStateEntryNode.h"
#include "Pixel2DTDAnimStateMachineGraph.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateMachineGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

	// Entry node within the state machine
	UPROPERTY()
	class UPixel2DTDAnimStateEntryNode* EntryNode;

	// Parent instance node
	UPROPERTY()
	class UPixel2DTDAnimGraphNode_StateMachineBase* OwnerAnimGraphNode;
};

