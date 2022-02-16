//Polyart Studio 2021


/*=============================================================================
Pixel2DTDAnimTransitionSchema.cpp
=============================================================================*/

#include "Pixel2DTDAnimTransitionSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimStateTransitionNode.h"
#include "Pixel2DTDAnimTransitionGraph.h"
#include "Pixel2DTDAnimGraphNode_TransitionResult.h"

/////////////////////////////////////////////////////
// UPixel2DTDAnimTransitionSchema

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimTransitionSchema"

UPixel2DTDAnimTransitionSchema::UPixel2DTDAnimTransitionSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialize defaults
}

void UPixel2DTDAnimTransitionSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Create the entry/exit tunnels
	FGraphNodeCreator<UPixel2DTDAnimGraphNode_TransitionResult> NodeCreator(Graph);
	UPixel2DTDAnimGraphNode_TransitionResult* ResultSinkNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(ResultSinkNode, FNodeMetadata::DefaultGraphNode);

	UPixel2DTDAnimTransitionGraph* TypedGraph = CastChecked<UPixel2DTDAnimTransitionGraph>(&Graph);
	TypedGraph->MyResultNode = ResultSinkNode;
}

void UPixel2DTDAnimTransitionSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());

	const UPixel2DTDAnimStateTransitionNode* TransNode = Cast<const UPixel2DTDAnimStateTransitionNode>(Graph.GetOuter());
	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

void UPixel2DTDAnimTransitionSchema::HandleGraphBeingDeleted(UEdGraph& GraphBeingRemoved) const
{
	Super::HandleGraphBeingDeleted(GraphBeingRemoved);

	if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(&GraphBeingRemoved))
	{
		// Handle composite anim graph nodes
		TArray<UPixel2DTDAnimStateNodeBase*> StateNodes;
		FBlueprintEditorUtils::GetAllNodesOfClassEx<UPixel2DTDAnimStateTransitionNode>(Blueprint, StateNodes);

		TSet<UPixel2DTDAnimStateNodeBase*> NodesToDelete;
		for (int32 i = 0; i < StateNodes.Num(); ++i)
		{
			UPixel2DTDAnimStateNodeBase* StateNode = StateNodes[i];
			if (StateNode->GetBoundGraph() == &GraphBeingRemoved)
			{
				NodesToDelete.Add(StateNode);
			}
		}

		// Delete the node that owns us
		ensure(NodesToDelete.Num() <= 1);
		for (TSet<UPixel2DTDAnimStateNodeBase*>::TIterator It(NodesToDelete); It; ++It)
		{
			UPixel2DTDAnimStateNodeBase* NodeToDelete = *It;

			FBlueprintEditorUtils::RemoveNode(Blueprint, NodeToDelete, true);

			// Prevent re-entrancy here
			NodeToDelete->ClearBoundGraph();
		}
	}
}

#undef LOCTEXT_NAMESPACE
