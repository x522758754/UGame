//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "BlueprintConnectionDrawingPolicy.h"

class FSlateWindowElementList;
class UEdGraph;

/////////////////////////////////////////////////////
// FAnimGraphConnectionDrawingPolicy

// This class draws the connections for an UEdGraph with an animation schema
class FPixel2DAnimGraphConnectionDrawingPolicy : public FKismetConnectionDrawingPolicy
{
public:
	// Constructor
	FPixel2DAnimGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);

	// FKismetConnectionDrawingPolicy interface
	virtual bool TreatWireAsExecutionPin(UEdGraphPin* InputPin, UEdGraphPin* OutputPin) const override;
	virtual void DetermineStyleOfExecWire(float& Thickness, FLinearColor& WireColor, bool& bDrawBubbles, const FTimePair& Times) override;
	// End of FKismetConnectionDrawingPolicy interface
};

