//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraph.h"
#include "Pixel2DTDAnimGraphNode_StateResult.h"
#include "Pixel2DTDAnimStateGraph.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateGraph : public UPixel2DTDAnimGraph
{
	GENERATED_UCLASS_BODY()

	// Result node within the state's animation graph
	UPROPERTY()
	class UPixel2DTDAnimGraphNode_StateResult* MyResultNode;

	PIXEL2DEDITORTD_API class UPixel2DTDAnimGraphNode_StateResult* GetResultNode();
};

