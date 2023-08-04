// Copyright Epic Games, Inc. All Rights Reserved.
#include "UI/SVirtualJoystick_Ex.h"
#include "Slate/DeferredCleanupSlateBrush.h"

int32 SVirtualJoystick_Ex::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayerId = LayerId;

	if (bVisible)
	{
		FLinearColor ColorAndOpacitySRGB = InWidgetStyle.GetColorAndOpacityTint();
		ColorAndOpacitySRGB.A = CurrentOpacity;
		for (int32 ControlIndex = 0; ControlIndex < Controls.Num(); ControlIndex++)
		{
			const FControlData& Control = Controls[ControlIndex];

			if (Control.Info.Image2.IsValid())
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					RetLayerId++,
					AllottedGeometry.ToPaintGeometry(
						Control.VisualCenter - FVector2D(Control.CorrectedVisualSize.X * 0.5f, Control.CorrectedVisualSize.Y * 0.5f),
						Control.CorrectedVisualSize),
					Control.Info.Image2->GetSlateBrush(),
					ESlateDrawEffect::None,
					ColorAndOpacitySRGB
				);
			}

			if (Control.Info.Image1.IsValid())
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					RetLayerId++,
					AllottedGeometry.ToPaintGeometry(
						Control.VisualCenter + Control.ThumbPosition - FVector2D(Control.CorrectedThumbSize.X * 0.5f, Control.CorrectedThumbSize.Y * 0.5f),
						Control.CorrectedThumbSize),
					Control.Info.Image1->GetSlateBrush(),
					ESlateDrawEffect::None,
					ColorAndOpacitySRGB
				);
			}

			if (Image3.IsValid())
			{
				FVector2D temp = Control.ThumbPosition;
				temp.Normalize();
				
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					RetLayerId++,
					AllottedGeometry.ToPaintGeometry(Control.CorrectedVisualSize,
						FSlateLayoutTransform(Control.VisualCenter - FVector2D(Control.CorrectedVisualSize.X * 0.5f, Control.CorrectedVisualSize.Y * 0.5f) + Control.ThumbPosition / 2),
						FSlateRenderTransform(FQuat2D(temp))),
					Image3->GetSlateBrush(),
					ESlateDrawEffect::None,
					ColorAndOpacitySRGB
				);           
			}
		}
	}

	return RetLayerId;
}

void SVirtualJoystick_Ex::SetDirectionTex(UTexture2D* Tex)
{
	Image3 = Tex ? StaticCastSharedRef<ISlateBrushSource>(FDeferredCleanupSlateBrush::CreateBrush(Tex)) : TSharedPtr<ISlateBrushSource>();
}

FVector2D SVirtualJoystick_Ex::GetThumbOffsetPerCent()
{
	if(Controls.Num() < 1)
	{
		return  FVector2D::ZeroVector;
	}
	FVector2D Result;
	const float Distance = Controls[0].CorrectedVisualSize.X - Controls[0].CorrectedThumbSize.X;
	Result.X = FMath::Clamp(Controls[0].ThumbPosition.X / Distance,-1.f,1.f);
	Result.Y = -FMath::Clamp(Controls[0].ThumbPosition.Y / Distance,-1.f,1.f);
	return Result;
	
}
