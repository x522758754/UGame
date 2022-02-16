//Polyart Studio 2021


#include "SPixel2DGraphNodeStateMachineInstance.h"
#include "Pixel2DTDAnimStateMachineGraph.h"
#include "Pixel2DTDAnimGraphNode_StateMachineBase.h"

/////////////////////////////////////////////////////
// SPixel2DGraphNodeStateMachineInstance

void SPixel2DGraphNodeStateMachineInstance::Construct(const FArguments& InArgs, UPixel2DTDAnimGraphNode_StateMachineBase* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

UEdGraph* SPixel2DGraphNodeStateMachineInstance::GetInnerGraph() const
{
	UPixel2DTDAnimGraphNode_StateMachineBase* StateMachineInstance = CastChecked<UPixel2DTDAnimGraphNode_StateMachineBase>(GraphNode);

	return StateMachineInstance->EditorStateMachineGraph;
}
