//Polyart Studio 2021


#include "Pixel2DTDAnimStateMachineGraph.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimGraph"

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateMachineGraph

UPixel2DTDAnimStateMachineGraph::UPixel2DTDAnimStateMachineGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowDeletion = false;
	bAllowRenaming = true;
}


#undef LOCTEXT_NAMESPACE
