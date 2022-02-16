//Polyart Studio 2021


#include "Pixel2DTDAnimStateGraphSchema.h"
#include "Pixel2DTDAnimStateGraph.h"
#include "Pixel2DTDAnimStateNode.h"
#include "Pixel2DTDAnimGraphNode_StateResult.h"
#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateGraphSchema"


/////////////////////////////////////////////////////
// UPixel2DTDAnimStateGraphSchema

UPixel2DTDAnimStateGraphSchema::UPixel2DTDAnimStateGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimStateGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Create the result Create
	FGraphNodeCreator<UPixel2DTDAnimGraphNode_StateResult> NodeCreator(Graph);
	UPixel2DTDAnimGraphNode_StateResult* ResultSinkNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(ResultSinkNode, FNodeMetadata::DefaultGraphNode);

	UPixel2DTDAnimStateGraph* TypedGraph = CastChecked<UPixel2DTDAnimStateGraph>(&Graph);
	TypedGraph->MyResultNode = ResultSinkNode;
}

void UPixel2DTDAnimStateGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
{
	DisplayInfo.PlainName = FText::FromString(Graph.GetName());

	if (const UPixel2DTDAnimStateNode* StateNode = Cast<const UPixel2DTDAnimStateNode>(Graph.GetOuter()))
	{
		DisplayInfo.PlainName = FText::Format(LOCTEXT("StateNameGraphTitle", "{0} (state)"), FText::FromString(StateNode->GetStateName()));
	}

	DisplayInfo.DisplayName = DisplayInfo.PlainName;
}

#undef LOCTEXT_NAMESPACE
