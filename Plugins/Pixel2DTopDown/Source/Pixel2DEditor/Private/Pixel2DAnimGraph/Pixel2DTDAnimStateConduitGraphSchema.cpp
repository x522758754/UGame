//Polyart Studio 2021


#include "Pixel2DTDAnimStateConduitGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimGraphNode_TransitionResult.h"
#include "Pixel2DTDAnimTransitionGraph.h"
#include "Pixel2DTDAnimStateConduitNode.h"

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateConduitGraphSchema

UPixel2DTDAnimStateConduitGraphSchema::UPixel2DTDAnimStateConduitGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimStateConduitGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UPixel2DTDAnimGraphNode_TransitionResult> NodeCreator(Graph);
	UPixel2DTDAnimGraphNode_TransitionResult* ResultSinkNode = NodeCreator.CreateNode();
	SetNodeMetaData(ResultSinkNode, FNodeMetadata::DefaultGraphNode);

	UPixel2DTDAnimTransitionGraph* TypedGraph = CastChecked<UPixel2DTDAnimTransitionGraph>(&Graph);
	TypedGraph->MyResultNode = ResultSinkNode;

	NodeCreator.Finalize();
}

void UPixel2DTDAnimStateConduitGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());

	if (const UPixel2DTDAnimStateConduitNode* ConduitNode = Cast<const UPixel2DTDAnimStateConduitNode>(Graph.GetOuter()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), ConduitNode->GetNodeTitle(ENodeTitleType::FullTitle));

		DisplayInfo.PlainName = FText::Format(NSLOCTEXT("Pixel2DAnimation", "ConduitRuleGraphTitle", "{NodeTitle} (conduit rule)"), Args);
	}

	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

void UPixel2DTDAnimStateConduitGraphSchema::HandleGraphBeingDeleted(UEdGraph& GraphBeingRemoved) const
{
	Super::HandleGraphBeingDeleted(GraphBeingRemoved);

	if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(&GraphBeingRemoved))
	{
		// Handle composite anim graph nodes
		TArray<UPixel2DTDAnimStateNodeBase*> StateNodes;
		FBlueprintEditorUtils::GetAllNodesOfClassEx<UPixel2DTDAnimStateConduitNode>(Blueprint, StateNodes);

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
