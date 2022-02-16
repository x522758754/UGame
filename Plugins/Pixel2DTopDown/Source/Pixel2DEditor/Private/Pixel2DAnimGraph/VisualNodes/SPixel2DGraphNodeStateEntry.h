//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "SGraphNode.h"

class SGraphPin;
class UPixel2DTDAnimStateEntryNode;

class SPixel2DGraphNodeStateEntry : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPixel2DGraphNodeStateEntry) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPixel2DTDAnimStateEntryNode* InNode);

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// End of SGraphNode interface

protected:
	FSlateColor GetBorderBackgroundColor() const;

	FText GetPreviewCornerText() const;
};
