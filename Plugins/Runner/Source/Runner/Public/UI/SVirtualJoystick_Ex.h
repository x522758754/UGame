// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SVirtualJoystick.h"

class SVirtualJoystick_Ex : public SVirtualJoystick
{
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
public:
	void SetDirectionTex(UTexture2D* Tex);
	FVector2D GetThumbOffsetPerCent();
private:
	TSharedPtr< ISlateBrushSource > Image3;
};
