//Polyart Studio 2021


#include "Pixel2DTDTileMapDestroyComponent.h"


UPixel2DTDTileMapDestroyComponent::UPixel2DTDTileMapDestroyComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDTileMapDestroyComponent::BeginPlay()
{
	Super::BeginPlay();

	// Promote the tilemap to an instance so that any change becomes
	// a local change
	AActor* Parent = GetOwner();
	if (Parent)
	{
		TArray<UPaperTileMapComponent*> TileMapComps;
		Parent->GetComponents<UPaperTileMapComponent>(TileMapComps);

		if (TileMapComps.Num() != 0)
		{
			TileMapComps[0]->MakeTileMapEditable();
		}
	}
}

void UPixel2DTDTileMapDestroyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	RestoreCollision();
}

void UPixel2DTDTileMapDestroyComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	RestoreCollision();
}

void UPixel2DTDTileMapDestroyComponent::RestoreCollision()
{
	if (HiddenCells.Num() == 0)
	{
		return;
	}

	AActor * Parent = GetOwner();
	if (Parent)
	{
		TArray<UPaperTileMapComponent*> TileMapComps;
		Parent->GetComponents<UPaperTileMapComponent>(TileMapComps);

		if (TileMapComps.Num() != 0)
		{
			RestoreHiddenTile(TileMapComps[0]->TileMap);
			if (TileMapComps[0]->TileMap)
			{
				TileMapComps[0]->TileMap->RebuildCollision();
			}
			TileMapComps[0]->RebuildCollision();
		}
	}
}