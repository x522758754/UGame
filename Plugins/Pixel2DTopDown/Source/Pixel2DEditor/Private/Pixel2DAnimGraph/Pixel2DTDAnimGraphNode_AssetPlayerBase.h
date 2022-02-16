//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraphNode_Base.h"
#include "Pixel2DTDAnimGraphNode_AssetPlayerBase.generated.h"

/** Get the default anim node class for playing a particular asset */
PIXEL2DEDITORTD_API UClass* GetNodeClassForAsset(const UClass* AssetClass);

/** See if a particular anim NodeClass can play a particular anim AssetClass */
PIXEL2DEDITORTD_API bool SupportNodeClassForAsset(const UClass* AssetClass, UClass* NodeClass);

class UPaperFlipbook;

/** Helper / intermediate for asset player graphical nodes */
UCLASS(Abstract, MinimalAPI)
class UPixel2DTDAnimGraphNode_AssetPlayerBase : public UPixel2DTDAnimGraphNode_Base
{
	GENERATED_BODY()
public:
	/** UEdGraphNode interface */
	PIXEL2DEDITORTD_API virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	PIXEL2DEDITORTD_API virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;

	virtual void SetAnimationAsset(UPaperFlipbook* Asset) { check(false); /*Base function called*/ }
};
