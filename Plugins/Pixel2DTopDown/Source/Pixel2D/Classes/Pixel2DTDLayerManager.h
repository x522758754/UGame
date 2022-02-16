//Polyart Studio 2021
 
#pragma once


#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"

#include "Pixel2DTDLayerManager.generated.h"

USTRUCT()
struct FPixel2DTDLayer
{
	FPixel2DTDLayer()
		: Name(), LayerID()
	{};

	FPixel2DTDLayer(FText LayerName, uint8 LayerID)
		: Name(LayerName), LayerID(LayerID)
	{};

	GENERATED_BODY()

	UPROPERTY(Category = PixelLayer, EditAnywhere)
	FText Name;

	UPROPERTY(Category = PixelLayer, EditAnywhere)
	uint8 LayerID;

	UPROPERTY(Category = PixelLayer, EditAnywhere)
	bool isVisible = true;

	UPROPERTY(Category = PixelLayer, EditAnywhere)
	bool isLocked = false;

};

UCLASS()
class PIXEL2DTD_API UPixel2DTDLayerManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	static void AddLayer();
	static void DeleteLayer(TSharedPtr<FPixel2DTDLayer> DeletedLayer);
	static void ChangeDepth(TSharedPtr<FPixel2DTDLayer> Layer, bool Increment);

	static int  GetNumLayers();
	static TArray<TSharedPtr<FPixel2DTDLayer>> GetLayers();
	static bool GetIsVisible(TSharedPtr<FPixel2DTDLayer> LayerName);
	static bool GetIsLocked(TSharedPtr<FPixel2DTDLayer> LayerName);

	static void SetIsVisible(bool bVisible);

	static void GetLayersByName(TArray<TSharedPtr<FString>> &Container);

public:
	static TArray<TSharedPtr<FPixel2DTDLayer>> Layers;
	static bool isVisible;

private:
	static uint16 FindDefaultDuplicates();
	static uint8  FindAvailableID();
};
