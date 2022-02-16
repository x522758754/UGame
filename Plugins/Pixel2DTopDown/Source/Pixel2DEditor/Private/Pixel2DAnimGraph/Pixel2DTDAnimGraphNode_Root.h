//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraphNode_Base.h"
#include "Pixel2DTDAnimNode_Root.h"
#include "Pixel2DTDAnimGraphNode_Root.generated.h"

class FBlueprintActionDatabaseRegistrar;

UCLASS(MinimalAPI)
class UPixel2DTDAnimGraphNode_Root : public UPixel2DTDAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FPixel2DTDAnimNode_Root Node;

	//~ Begin UEdGraphNode Interface.
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false; }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UPixel2DTDAnimGraphNode_Base Interface
	virtual bool IsSinkNode() const override;

	//~ End UPixel2DTDAnimGraphNode_Base Interface
};
