//Polyart Studio 2021 


#include "Pixel2DTDTileMapFactory.h"
#include "Pixel2DTDTileMap.h"
#include "Pixel2DTDImporterSettings.h"


#define LOCTEXT_NAMESPACE "Pixel2DTopDown"

/////////////////////////////////////////////////////
// UPixel2DTDTileMapFactory

UPixel2DTDTileMapFactory::UPixel2DTDTileMapFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UPixel2DTDTileMap::StaticClass();
}

UObject* UPixel2DTDTileMapFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UPixel2DTDTileMap* NewTileMap = NewObject<UPixel2DTDTileMap>(InParent, Class, Name, Flags | RF_Transactional);

	GetDefault<UPixel2DTDImporterSettings>()->ApplySettingsForTileMapInit(NewTileMap, InitialTileSet);

	return NewTileMap;
}

#undef LOCTEXT_NAMESPACE
