//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UK2Node_PixelTDTileHide.generated.h"


class FBlueprintActionDatabaseRegistrar;
class UPaperTileMap;
class APaperTileMapActor;
class UPaperTileMapComponent;
class UPixel2DTDTileMapDestroyComponent;

UENUM(BlueprintType)
enum class EPixel2DTDTileDestoryError : uint8
{
	E_OK,
	E_INVALID_ACTOR,
	E_INVALID_TILEMAP,
	E_INVALID_TILEDESTORY,
	E_INVALID_INDEX
};

UCLASS()
class PIXEL2DTD_API UK2Node_PixelTDTileHide : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Pixel2DTopDown|TileDestroy", meta = (AdvancedDisplay = 4))
	static void HideActorTile(APaperTileMapActor * TileMapActor, int Layer, int PosX, int PosY, bool RebuildCollision = false);

	UFUNCTION(BlueprintCallable, Category = "Pixel2DTopDown|TileDestroy")
	static void  RestoreTileMap(APaperTileMapActor * TileMapActor);

	UFUNCTION(BlueprintCallable, Category = "Pixel2DTopDown|TileDestroy")
	static void  RebuildCollisionBox(APaperTileMapActor * TileMapActor);


private:
	static bool CheckData(const APaperTileMapActor * InTileMapActor, TArray<UPixel2DTDTileMapDestroyComponent*> & OutStaticComps);
};
