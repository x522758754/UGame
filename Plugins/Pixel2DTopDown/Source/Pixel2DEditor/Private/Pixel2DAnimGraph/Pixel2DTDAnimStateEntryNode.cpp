//Polyart Studio 2021


/*=============================================================================
Pixel2DTDAnimStateNode.cpp
=============================================================================*/

#include "Pixel2DTDAnimStateEntryNode.h"
#include "EdGraph/EdGraph.h"
#include "Pixel2DTDAnimStateMachineSchema.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateEntryNode"

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateEntryNode

UPixel2DTDAnimStateEntryNode::UPixel2DTDAnimStateEntryNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimStateEntryNode::AllocateDefaultPins()
{
	const UPixel2DTDAnimStateMachineSchema* Schema = GetDefault<UPixel2DTDAnimStateMachineSchema>();
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, Schema->PC_Exec, TEXT("Entry"));
}

FText UPixel2DTDAnimStateEntryNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UEdGraph* Graph = GetGraph();
	return FText::FromString(Graph->GetName());
}

FText UPixel2DTDAnimStateEntryNode::GetTooltipText() const
{
	return LOCTEXT("StateEntryNodeTooltip", "Entry point for state machine");
}

UEdGraphNode* UPixel2DTDAnimStateEntryNode::GetOutputNode() const
{
	if (Pins.Num() > 0 && Pins[0] != NULL)
	{
		check(Pins[0]->LinkedTo.Num() <= 1);
		if (Pins[0]->LinkedTo.Num() > 0 && Pins[0]->LinkedTo[0]->GetOwningNode() != NULL)
		{
			return Pins[0]->LinkedTo[0]->GetOwningNode();
		}
	}
	return NULL;
}

#undef LOCTEXT_NAMESPACE
