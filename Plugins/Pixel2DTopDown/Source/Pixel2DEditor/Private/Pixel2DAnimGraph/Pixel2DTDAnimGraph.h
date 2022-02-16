//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "AnimationGraph.h"
#include "Pixel2DTDAnimGraph.generated.h"

class UEdGraphPin;

/** Delegate fired when a pin's default value is changed */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPinDefaultValueChanged, UEdGraphPin* /*InPinThatChanged*/)

UCLASS(MinimalAPI)
class UPixel2DTDAnimGraph : public UAnimationGraph
{
	GENERATED_UCLASS_BODY()

	/** Delegate fired when a pin's default value is changed */
	FOnPinDefaultValueChanged OnPinDefaultValueChanged;
};

