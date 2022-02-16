//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditorModes.h"
#include "Pixel2DAnimBlueprintEditor.h"
#include "Pixel2DTDAnimBlueprint.h"

class FPixel2DAnimBlueprintEditorMode : public FBlueprintEditorApplicationMode
{
protected:
	// Set of spawnable tabs in persona mode (@TODO: Multiple lists!)
	FWorkflowAllowedTabSet TabFactories;

public:
	FPixel2DAnimBlueprintEditorMode(const TSharedRef<FPixel2DAnimBlueprintEditor>& InAnimationBlueprintEditor);

	// FApplicationMode interface
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	// End of FApplicationMode interface

private:
	TWeakObjectPtr<class UPixel2DTDAnimBlueprint> AnimBlueprintPtr;
};