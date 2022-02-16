//Polyart Studio 2021 


#include "Pixel2DTDTileMapPromotionFactory.h"
#include "Pixel2DTDTileMap.h"

#define LOCTEXT_NAMESPACE "Pixel2DTopDown"

/////////////////////////////////////////////////////
// UPixel2DTDTileMapPromotionFactory

UPixel2DTDTileMapPromotionFactory::UPixel2DTDTileMapPromotionFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	bEditAfterNew = true;
	SupportedClass = UPixel2DTDTileMap::StaticClass();
}

UObject* UPixel2DTDTileMapPromotionFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	AssetToRename->SetFlags(Flags | RF_Transactional);
	AssetToRename->Modify();
	AssetToRename->Rename(*Name.ToString(), InParent);

	return AssetToRename;
}

#undef LOCTEXT_NAMESPACE
