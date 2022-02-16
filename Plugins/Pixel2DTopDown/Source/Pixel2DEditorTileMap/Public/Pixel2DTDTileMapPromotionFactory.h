//Polyart Studio 2021


#pragma once

/**
 * Factory used to promote an instance-specific object from instance to asset by renaming it into an asset package
 */

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "Pixel2DTDTileMapPromotionFactory.generated.h"

UCLASS()
class UPixel2DTDTileMapPromotionFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// Object being promoted to an asset
	UPROPERTY()
	class UPixel2DTDTileMap* AssetToRename;

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
