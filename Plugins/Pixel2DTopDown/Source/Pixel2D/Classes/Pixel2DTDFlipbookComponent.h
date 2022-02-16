//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "PaperFlipbookComponent.h"
#include "Pixel2DTDFlipbookComponent.generated.h"

UENUM()
namespace EPixelFlipbookPivot
{
	enum Type
	{
		Top_Left,
		Top_Center,
		Top_Right,
		Center_Left,
		Center_Center,
		Center_Right,
		Bottom_Left,
		Bottom_Center,
		Bottom_Right,
		CustomOffset,
		Learned
	};
}

UCLASS(ShowCategories = (Mobility, ComponentReplication), meta = (BlueprintSpawnableComponent))
class PIXEL2DTD_API UPixel2DTDFlipbookComponent : public UPaperFlipbookComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Category=Bounds, EditAnywhere)
	bool bAdjustBounds;

	// Pivot mode
	UPROPERTY(Category = Bounds, EditAnywhere)
	TEnumAsByte<EPixelFlipbookPivot::Type> PivotMode;

	UPROPERTY(Category = Bounds, EditAnywhere)
	FVector2D CustomPivotPoint;

	UPROPERTY(Category = Bounds, BlueprintReadOnly)
	FVector2D LearnedPivotPoint;

public:
	//FlipbookComponent interface
	virtual bool SetFlipbook(class UPaperFlipbook* NewFlipbook) override;
	//
	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//~ End UObject Interface


	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual FBoxSphereBounds CalcTrueBounds(const FTransform& LocalToWorld) const;

private:
	FVector GetPivotLocation(const FBoxSphereBounds OriginalBounds) const;

#if WITH_EDITOR
	void RefreshPivotPoint(UPaperFlipbook* Flipbook);
#endif
};