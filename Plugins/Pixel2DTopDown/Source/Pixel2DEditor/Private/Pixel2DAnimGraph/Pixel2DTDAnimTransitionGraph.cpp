//Polyart Studio 2021


#include "Pixel2DTDAnimTransitionGraph.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateGraph"

/////////////////////////////////////////////////////
// UPixel2DTDAnimTransitionGraph

UPixel2DTDAnimTransitionGraph::UPixel2DTDAnimTransitionGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPixel2DTDAnimGraphNode_TransitionResult* UPixel2DTDAnimTransitionGraph::GetResultNode()
{
	return MyResultNode;
}

#undef LOCTEXT_NAMESPACE
