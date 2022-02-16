//Polyart Studio 2021


#include "Pixel2DTDAnimStateGraph.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateGraph"

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateGraph

UPixel2DTDAnimStateGraph::UPixel2DTDAnimStateGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPixel2DTDAnimGraphNode_StateResult* UPixel2DTDAnimStateGraph::GetResultNode()
{
	return MyResultNode;
}

#undef LOCTEXT_NAMESPACE
