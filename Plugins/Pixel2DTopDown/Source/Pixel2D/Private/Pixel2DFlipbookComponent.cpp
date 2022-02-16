//Polyart Studio 2021


#include "Pixel2DTDFlipbookComponent.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"

UPixel2DTDFlipbookComponent::UPixel2DTDFlipbookComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bAdjustBounds{true}
	, PivotMode{EPixelFlipbookPivot::Type::Learned}
{
}

bool UPixel2DTDFlipbookComponent::SetFlipbook(class UPaperFlipbook* NewFlipbook)
{
	bool bUpdatedFlipbook = Super::SetFlipbook(NewFlipbook);

#if WITH_EDITOR
	if (bUpdatedFlipbook)
	{
		RefreshPivotPoint(NewFlipbook);
	}
#endif

	return bUpdatedFlipbook;
}

FBoxSphereBounds UPixel2DTDFlipbookComponent::CalcBounds(const FTransform& LocalToWorld) const
{

	const FBoxSphereBounds NewBounds = Super::CalcBounds(LocalToWorld);

	if ((SourceFlipbook != nullptr) && bAdjustBounds)
	{
		FVector Pivot;
		if (PivotMode != EPixelFlipbookPivot::Learned)
		{
			Pivot = GetPivotLocation(NewBounds);
		}
		else
		{
			Pivot = LocalToWorld.TransformPosition({ LearnedPivotPoint.X,
				LearnedPivotPoint.Y, 0 });
		}

		FVector DistanceToPivot = Pivot - NewBounds.Origin;

		FBoxSphereBounds ExtendedBounds = NewBounds;
		ExtendedBounds.Origin = Pivot;
		ExtendedBounds.BoxExtent += (DistanceToPivot.GetAbs());

		return ExtendedBounds;
	}
	else
	{
		return NewBounds;
	}
}

FVector UPixel2DTDFlipbookComponent::GetPivotLocation(const FBoxSphereBounds OriginalBounds) const
{
	FVector Origin = OriginalBounds.Origin;
	FVector Extent = OriginalBounds.BoxExtent;
	FVector2D Pivot(Origin.X, Origin.Y);
	FVector2D TopLeft(Origin.X - Extent.X, Origin.Y - Extent.Y);

	switch (PivotMode)
	{
		case EPixelFlipbookPivot::Top_Left:
			Pivot.X = Origin.X - Extent.X;
			Pivot.Y = Origin.Y - Extent.Y;
			break;
		case EPixelFlipbookPivot::Top_Center:
			Pivot.X = Origin.X;
			Pivot.Y = Origin.Y - Extent.Y;
			break;
		case EPixelFlipbookPivot::Top_Right:
			Pivot.X = Origin.X + Extent.X;
			Pivot.Y = Origin.Y - Extent.Y;
			break;
		case EPixelFlipbookPivot::Center_Left:
			Pivot.X = Origin.X - Extent.X;
			Pivot.Y = Origin.Y;
			break;
		case EPixelFlipbookPivot::Center_Center:
			Pivot.X = Origin.X;
			Pivot.Y = Origin.Y;
			break;
		case EPixelFlipbookPivot::Center_Right:
			Pivot.X = Origin.X + Extent.X;
			Pivot.Y = Origin.Y;
			break;
		case EPixelFlipbookPivot::Bottom_Left:
			Pivot.X = Origin.X - Extent.X;
			Pivot.Y = Origin.Y + Extent.Y;
			break;
		case EPixelFlipbookPivot::Bottom_Center:
			Pivot.X = Origin.X;
			Pivot.Y = Origin.Y + Extent.Y;
			break;
		case EPixelFlipbookPivot::Bottom_Right:
			Pivot.X = Origin.X + Extent.X;
			Pivot.Y = Origin.Y + Extent.Y;
			break;
		case EPixelFlipbookPivot::CustomOffset:
			Pivot.X = Origin.X + CustomPivotPoint.X;
			Pivot.Y = Origin.Y + CustomPivotPoint.Y;
			break;
		case EPixelFlipbookPivot::Learned:
			Pivot.X = LearnedPivotPoint.X;
			Pivot.Y = LearnedPivotPoint.Y;
			break;
	};

	return FVector(Pivot.X, Pivot.Y, Origin.Z);
}
FBoxSphereBounds UPixel2DTDFlipbookComponent::CalcTrueBounds(const FTransform& LocalToWorld) const
{
	return Super::CalcBounds(LocalToWorld);
}

#if WITH_EDITOR
void UPixel2DTDFlipbookComponent::RefreshPivotPoint(UPaperFlipbook* Flipbook)
{
	if (Flipbook && Flipbook->GetNumFrames())
	{
		const UPaperSprite* FirstSprite = Flipbook->GetSpriteAtFrame(0);
		FVector2D SourcePivot = FirstSprite->GetPivotPosition();
		// REVISIT - Pivot is center in the sprite reference space
		LearnedPivotPoint = { 0, 0 };
	}
}

void UPixel2DTDFlipbookComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshPivotPoint(SourceFlipbook);
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif