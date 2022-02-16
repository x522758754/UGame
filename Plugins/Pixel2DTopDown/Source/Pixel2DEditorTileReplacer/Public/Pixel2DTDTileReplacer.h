//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

#include "Pixel2DTDTileReplacer.generated.h"

class UPaperTileSet;
class UPaperTileMap;
class UPaperFlipbook;
struct FPaperTileInfo;
class APaperTileMapActor;
class APaperFlipbookActor;

USTRUCT(BlueprintType)
struct PIXEL2DEDITORTILEREPLACERTD_API FPixel2DTDTileReplacerRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	TArray<TSoftObjectPtr<UWorld>> TargetWorlds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	TArray<TSoftObjectPtr<UPaperTileMap>> TargetTileMaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	TSoftObjectPtr<UPaperTileSet> TileSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	int TileSetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	TSoftObjectPtr<UPaperFlipbook> Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	bool bSpawnActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileReplacerRow")
	bool bLockActorRelativePosition;

};

UCLASS()
class UPixel2DTDFlipbookTilePositionComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	FVector TileCenterPosition;
};

/* Class responsible for replacing tiles in a tilemap with actors/components
 * The actual replacing happens by hiding each tile that matches a mapping table
 * and spawing either an actor or component at its location.
 */
class FPixel2DTDTileReplacer
{

public:
	FPixel2DTDTileReplacer();
	~FPixel2DTDTileReplacer();

	void Initialize();
	void Uninitialize();

private:

	/* Flag to guard against recursive calls caused by replacing a tile with a tilemap actor*/
	bool bAlreadyProcessing;

	/* Table containing the replacing information */
	UDataTable * MappingTable;

	/* Keeps track of the spawned actors that need to be grouped */
	TArray<AActor*> AddedActors;

private:
	void RegisterActorSpawnHandler();
	void OnActorDragged(const TArray<UObject*>& InObj, const TArray<AActor*>& InActor);

	/** Indicates whether a certain tile is marked as "replaceable" in the mappings table
	 * @param TileInfo - candidate tile for replacement
	 * @param CurrentWorldMapName - level/map name of the current world
	 * @param CurrentTileMapName - name of the target tilemap
	 * @param TableRow - current row of the mappings table against which TileInfo should be checked
	 * @return True if the TileInfo should be replaced by a component/actor
	 */
	bool ShouldReplaceTile(const FPaperTileInfo & TileInfo, const FString& CurrentWorldMapName, const FString& CurrentTileMapName, const FPixel2DTDTileReplacerRow * TableRow);

	/* Replaces Actor's TileInfo at position TilePosition with a component/actor as indicated by TableRow */
	void ReplaceTile(const FPaperTileInfo & TileInfo, const FPixel2DTDTileReplacerRow * TableRow, APaperTileMapActor* Actor, const FVector &TilePosition);

	/* Spawns an independent Flipbook actor at the TilePosition (coordinates in the Actor's system of coordinates) */
	APaperFlipbookActor* SpawnActor(APaperTileMapActor* Actor, const FVector &TilePosition, UPaperFlipbook * Flipbook, bool bLockPosition);

	/* Spawns a Flipbook component at the TilePosition and attaches it to the Actor */
	void SpawnComponent(APaperTileMapActor* Actor, const FVector &TilePosition, UPaperFlipbook * Flipbook);

	/* Checks whether any of the TileMap's tiles are marked as replacaeble in the mappings table*/
	bool HasReplaceableTiles(const UPaperTileMap * TileMap, const FString& CurrentMapName);
};