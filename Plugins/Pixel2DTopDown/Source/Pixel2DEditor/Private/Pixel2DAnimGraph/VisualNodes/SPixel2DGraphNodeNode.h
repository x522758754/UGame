//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "KismetNodes/SGraphNodeK2Base.h"

class UPixel2DTDAnimGraphNode_Base;

class SPixel2DGraphNodeNode : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SPixel2DGraphNodeNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPixel2DTDAnimGraphNode_Base* InNode);

protected:

	// SGraphNode interface
	virtual TArray<FOverlayWidgetInfo> GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const override;
	// End of SGraphNode interface

private:
	/** Keep a reference to the indicator widget handing around */
	TSharedPtr<SWidget> IndicatorWidget;
};
