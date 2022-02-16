//Polyart Studio 2021


#pragma once

/**
 * Factory for tile maps
 */

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "Pixel2DTDTileMapFactory.generated.h"

UCLASS()
class UPixel2DTDTileMapFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// Initial tile set to create the tile map from (Can be nullptr)
	class UPaperTileSet* InitialTileSet;

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
