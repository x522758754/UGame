//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraph.h"
#include "Pixel2DTDAnimTransitionGraph.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimTransitionGraph : public UPixel2DTDAnimGraph
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	class UPixel2DTDAnimGraphNode_TransitionResult* MyResultNode;

	PIXEL2DEDITORTD_API class UPixel2DTDAnimGraphNode_TransitionResult* GetResultNode();
};

