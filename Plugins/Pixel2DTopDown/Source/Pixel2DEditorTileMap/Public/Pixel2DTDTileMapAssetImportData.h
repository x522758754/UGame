//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Texture.h"
#include "EditorFramework/AssetImportData.h"
#include "Pixel2DTDTileMapAssetImportData.generated.h"

class UPaperTileSet;

USTRUCT()
struct FPixel2DTDTileSetImportMapping
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString SourceName;

	UPROPERTY()
	TWeakObjectPtr<class UPaperTileSet> ImportedTileSet;

	UPROPERTY()
	TWeakObjectPtr<class UTexture> ImportedTexture;
};

/**
 * Base class for import data and options used when importing a tile map
 */
UCLASS()
class PIXEL2DEDITORTILEMAPTD_API UPixel2DTDTileMapAssetImportData : public UAssetImportData
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	TArray<FPixel2DTDTileSetImportMapping> TileSetMap;

	static UPixel2DTDTileMapAssetImportData* GetImportDataForTileMap(class UPixel2DTDTileMap* TileMap);
};
