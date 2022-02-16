//Polyart Studio 2021 


#include "Pixel2DTDTileMapAssetImportData.h"
#include "Pixel2DTDTileMap.h"

///////////////////////////////////////////////////////////////////////////
// UPixel2DTDTileMapAssetImportData

UPixel2DTDTileMapAssetImportData::UPixel2DTDTileMapAssetImportData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPixel2DTDTileMapAssetImportData* UPixel2DTDTileMapAssetImportData::GetImportDataForTileMap(UPixel2DTDTileMap* TileMap/*, UFbxSkeletalMeshImportData* TemplateForCreation*/)
{
	check(TileMap);

	UPixel2DTDTileMapAssetImportData* ImportData = Cast<UPixel2DTDTileMapAssetImportData>(TileMap->AssetImportData);
	if (ImportData == nullptr)
	{
		ImportData = NewObject<UPixel2DTDTileMapAssetImportData>(TileMap, NAME_None, RF_NoFlags/*, TemplateForCreation*/);

		// Try to preserve the source file path if possible
		if (TileMap->AssetImportData != nullptr)
		{
			ImportData->SourceData = TileMap->AssetImportData->SourceData;
		}

		TileMap->AssetImportData = ImportData;
	}

	return ImportData;
}
