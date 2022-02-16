//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphNode.h"
#include "Pixel2DTDAnimStateEntryNode.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateEntryNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()


	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface

	PIXEL2DEDITORTD_API UEdGraphNode* GetOutputNode() const;

};
