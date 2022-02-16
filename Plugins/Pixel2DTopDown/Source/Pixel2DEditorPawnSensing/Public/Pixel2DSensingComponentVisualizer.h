//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class FPrimitiveDrawInterface;
class FSceneView;

class PIXEL2DEDITORPAWNSENSINGTD_API FPixel2DSensingComponentVisualizer : public FComponentVisualizer
{
public:
	//~ Begin FComponentVisualizer Interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	//~ End FComponentVisualizer Interface
};
