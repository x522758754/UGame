//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimNode_StateMachine.h"
#include "Pixel2DTDAnimGraphNode_StateMachineBase.h"
#include "Pixel2DTDAnimGraphNode_StateMachine.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimGraphNode_StateMachine : public UPixel2DTDAnimGraphNode_StateMachineBase
{
	GENERATED_UCLASS_BODY()

	// Runtime state machine representation (empty; populated during compilation)
	UPROPERTY(EditAnywhere, Category = Settings)
	FPixel2DTDAnimNode_StateMachine Node;

	// UPixel2DTDAnimGraphNode_StateMachineBase interface
	virtual FPixel2DTDAnimNode_StateMachine& GetNode() override { return Node; }
	// End of UAnimGraphNode_StateMachineBase interface
};
