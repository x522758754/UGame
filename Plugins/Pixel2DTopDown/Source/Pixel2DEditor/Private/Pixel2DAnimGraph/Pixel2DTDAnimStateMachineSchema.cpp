 //Polyart Studio 2021


/*=============================================================================
UPixel2DTDAnimStateMachineSchema.cpp
=============================================================================*/

#include "Pixel2DTDAnimStateMachineSchema.h"
#include "EdGraph/EdGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphNode_Comment.h"

#include "Pixel2DTDAnimStateMachineGraph.h"
#include "Pixel2DTDAnimGraphSchema.h"
#include "Pixel2DTDAnimStateConduitNode.h"
#include "Pixel2DTDAnimStateEntryNode.h"
#include "Pixel2DTDAnimStateNodeBase.h"
#include "Pixel2DTDAnimStateNode.h"
#include "Pixel2DTDAnimStateTransitionNode.h"
#include "GraphEditorActions.h"
#include "ToolMenus/Public/ToolMenu.h"

#include "EdGraphUtilities.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateMachineSchema"

// Helper function

TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateNode> AddNewStateNodeAction(FGraphContextMenuBuilder& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip, const int32 Grouping = 0)
{
	TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateNode> NewStateNode(new FPixel2DTDGraphSchemaAction_NewStateNode(Category, MenuDesc, Tooltip, Grouping));
	ContextMenuBuilder.AddAction(NewStateNode);
	return NewStateNode;
}

/////////////////////////////////////////////////////
// FPixel2DTDGraphSchemaAction_NewStateNode

UEdGraphNode* FPixel2DTDGraphSchemaAction_NewStateNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode/* = true*/)
{
	UEdGraphNode* ResultNode = NULL;

	// If there is a template, we actually use it
	if (NodeTemplate != NULL)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "K2_AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		// set outer to be the graph so it doesn't go away
		NodeTemplate->Rename(NULL, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;
		//@TODO: ANIM: SNAP_GRID isn't centralized or exposed - NodeTemplate->SnapToGrid(SNAP_GRID);

		ResultNode = NodeTemplate;

		ResultNode->SetFlags(RF_Transactional);

		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(ParentGraph);
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	}

	return ResultNode;
}

void FPixel2DTDGraphSchemaAction_NewStateNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);

	// These don't get saved to disk, but we want to make sure the objects don't get GC'd while the action array is around
	Collector.AddReferencedObject(NodeTemplate);
}

/////////////////////////////////////////////////////
// FPixel2DTDGraphSchemaAction_NewStateComment

UEdGraphNode* FPixel2DTDGraphSchemaAction_NewStateComment::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode/* = true*/)
{
	// Add menu item for creating comment boxes
	UEdGraphNode_Comment* CommentTemplate = NewObject<UEdGraphNode_Comment>();

	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(ParentGraph);

	FVector2D SpawnLocation = Location;

	FSlateRect Bounds;
	if ((Blueprint != NULL) && FKismetEditorUtilities::GetBoundsForSelectedNodes(Blueprint, Bounds, 50.0f))
	{
		CommentTemplate->SetBounds(Bounds);
		SpawnLocation.X = CommentTemplate->NodePosX;
		SpawnLocation.Y = CommentTemplate->NodePosY;
	}

	return FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation);
}

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateMachineSchema

const FName UPixel2DTDAnimStateMachineSchema::PC_Exec(TEXT("exec"));

UPixel2DTDAnimStateMachineSchema::UPixel2DTDAnimStateMachineSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimStateMachineSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Create the entry/exit tunnels
	FGraphNodeCreator<UPixel2DTDAnimStateEntryNode> NodeCreator(Graph);
	UPixel2DTDAnimStateEntryNode* EntryNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);

	if (UPixel2DTDAnimStateMachineGraph* StateMachineGraph = CastChecked<UPixel2DTDAnimStateMachineGraph>(&Graph))
	{
		StateMachineGraph->EntryNode = EntryNode;
	}
}

const FPinConnectionResponse UPixel2DTDAnimStateMachineSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}

	// Connect entry node to a state is OK
	const bool bPinAIsEntry = PinA->GetOwningNode()->IsA(UPixel2DTDAnimStateEntryNode::StaticClass());
	const bool bPinBIsEntry = PinB->GetOwningNode()->IsA(UPixel2DTDAnimStateEntryNode::StaticClass());
	const bool bPinAIsStateNode = PinA->GetOwningNode()->IsA(UPixel2DTDAnimStateNodeBase::StaticClass());
	const bool bPinBIsStateNode = PinB->GetOwningNode()->IsA(UPixel2DTDAnimStateNodeBase::StaticClass());

	if (bPinAIsEntry || bPinBIsEntry)
	{
		if (bPinAIsEntry && bPinBIsStateNode)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
		}

		if (bPinBIsEntry && bPinAIsStateNode)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Entry must connect to a state node"));
	}

	const bool bPinAIsTransition = PinA->GetOwningNode()->IsA(UPixel2DTDAnimStateTransitionNode::StaticClass());
	const bool bPinBIsTransition = PinB->GetOwningNode()->IsA(UPixel2DTDAnimStateTransitionNode::StaticClass());

	if (bPinAIsTransition && bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot wire a transition to a transition"));
	}

	// Compare the directions
	bool bDirectionsOK = false;

	if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}
	else if ((PinB->Direction == EGPD_Input) && (PinA->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}

	//if (!bDirectionsOK)
	//{
	//    return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Directions are not compatible"));
	//}

	// Transitions are exclusive (both input and output), but states are not
	if (bPinAIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	}
	else if (bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	}
	else if (!bPinAIsTransition && !bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT("Create a transition"));
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}
}

bool UPixel2DTDAnimStateMachineSchema::TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	if (PinB->Direction == PinA->Direction)
	{
		if (UPixel2DTDAnimStateNodeBase* Node = Cast<UPixel2DTDAnimStateNodeBase>(PinB->GetOwningNode()))
		{
			if (PinA->Direction == EGPD_Input)
			{
				PinB = Node->GetOutputPin();
			}
			else
			{
				PinB = Node->GetInputPin();
			}
		}
	}

	const bool bModified = UEdGraphSchema::TryCreateConnection(PinA, PinB);

	if (bModified)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(PinA->GetOwningNode());
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}

	return bModified;
}

bool UPixel2DTDAnimStateMachineSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	UPixel2DTDAnimStateNodeBase* NodeA = Cast<UPixel2DTDAnimStateNodeBase>(PinA->GetOwningNode());
	UPixel2DTDAnimStateNodeBase* NodeB = Cast<UPixel2DTDAnimStateNodeBase>(PinB->GetOwningNode());

	if ((NodeA != NULL) && (NodeB != NULL)
		&& (NodeA->GetInputPin() != NULL) && (NodeA->GetOutputPin() != NULL)
		&& (NodeB->GetInputPin() != NULL) && (NodeB->GetOutputPin() != NULL))
	{
		UPixel2DTDAnimStateTransitionNode* TransitionNode = FPixel2DTDGraphSchemaAction_NewStateNode::SpawnNodeFromTemplate<UPixel2DTDAnimStateTransitionNode>(NodeA->GetGraph(), NewObject<UPixel2DTDAnimStateTransitionNode>(), FVector2D(0.0f, 0.0f), false);

		if (PinA->Direction == EGPD_Output)
		{
			TransitionNode->CreateConnections(NodeA, NodeB);
		}
		else
		{
			TransitionNode->CreateConnections(NodeB, NodeA);
		}

		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(TransitionNode->GetBoundGraph());
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

		return true;
	}

	return false;
}

void UPixel2DTDAnimStateMachineSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// Add state node
	{
		TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateNode> Action = AddNewStateNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddState", "Add State..."), LOCTEXT("AddStateTooltip", "A new state"));
		Action->NodeTemplate = NewObject<UPixel2DTDAnimStateNode>(ContextMenuBuilder.OwnerOfTemporaries);
	}

	// Add conduit node
	{
		TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateNode> Action = AddNewStateNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddConduit", "Add Conduit..."), LOCTEXT("AddConduitTooltip", "A new conduit state"));
		Action->NodeTemplate = NewObject<UPixel2DTDAnimStateConduitNode>(ContextMenuBuilder.OwnerOfTemporaries);
	}

	// Entry point (only if doesn't already exist)
	{
		bool bHasEntry = false;
		for (auto NodeIt = ContextMenuBuilder.CurrentGraph->Nodes.CreateConstIterator(); NodeIt; ++NodeIt)
		{
			UEdGraphNode* Node = *NodeIt;
			if (const UPixel2DTDAnimStateEntryNode* StateNode = Cast<UPixel2DTDAnimStateEntryNode>(Node))
			{
				bHasEntry = true;
				break;
			}
		}

		if (!bHasEntry)
		{
			TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateNode> Action = AddNewStateNodeAction(ContextMenuBuilder, FText::GetEmpty(), LOCTEXT("AddEntryPoint", "Add Entry Point..."), LOCTEXT("AddEntryPointTooltip", "Define State Machine's Entry Point"));
			Action->NodeTemplate = NewObject<UPixel2DTDAnimStateEntryNode>(ContextMenuBuilder.OwnerOfTemporaries);
		}
	}

	// Add Comment
	if (!ContextMenuBuilder.FromPin)
	{
		UBlueprint* OwnerBlueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(ContextMenuBuilder.CurrentGraph);
		const bool bIsManyNodesSelected = (FKismetEditorUtilities::GetNumberOfSelectedNodes(OwnerBlueprint) > 0);
		const FText MenuDescription = bIsManyNodesSelected ? LOCTEXT("CreateCommentSelection", "Create Comment from Selection") : LOCTEXT("AddComment", "Add Comment...");
		const FText ToolTip = LOCTEXT("CreateCommentSelectionTooltip", "Create a resizeable comment box around selected nodes.");

		TSharedPtr<FPixel2DTDGraphSchemaAction_NewStateComment> NewComment(new FPixel2DTDGraphSchemaAction_NewStateComment(FText::GetEmpty(), MenuDescription, ToolTip, 0));
		ContextMenuBuilder.AddAction(NewComment);
	}
}

EGraphType UPixel2DTDAnimStateMachineSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UPixel2DTDAnimStateMachineSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	check(Context->Graph);
	UBlueprint* OwnerBlueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(Context->Graph);

	if (Context->Node != NULL)
	{
		FToolMenuSection& Section = Menu->AddSection("SpriteAnimationStateMachineNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			if (!Context->bIsDebugging)
			{
				// Node contextual actions
				Section.AddMenuEntry(FGenericCommands::Get().Delete);
				Section.AddMenuEntry(FGenericCommands::Get().Cut);
				Section.AddMenuEntry(FGenericCommands::Get().Copy);
				Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
				Section.AddMenuEntry(FGraphEditorCommands::Get().ReconstructNodes);
				Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
				if (Context->Node->bCanRenameNode)
				{
					Section.AddMenuEntry(FGenericCommands::Get().Rename);
				}
			}
		}
	}

	Super::GetContextMenuActions(Menu, Context);
}

FLinearColor UPixel2DTDAnimStateMachineSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

void UPixel2DTDAnimStateMachineSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());
	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

void UPixel2DTDAnimStateMachineSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));

	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(&TargetNode);
	Super::BreakNodeLinks(TargetNode);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
}

void UPixel2DTDAnimStateMachineSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));
	// cache this here, as BreakPinLinks can trigger a node reconstruction invalidating the TargetPin references
	UBlueprint* const Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(TargetPin.GetOwningNode());
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
}

void UPixel2DTDAnimStateMachineSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
    const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));
    UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(TargetPin->GetOwningNode());
    Super::BreakSinglePinLink(SourcePin, TargetPin);
    FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
}

#undef LOCTEXT_NAMESPACE
