//Polyart Studio 2021 


#include "Pixel2DTDTileMapActorFactory.h"
#include "AssetData.h"
#include "Pixel2DTDTileMapActor.h"
#include "Pixel2DTDTileMap.h"
#include "Pixel2DTDImporterSettings.h"
#include "Pixel2DTDTileMapComponent.h"
#include "PaperTileSet.h"

//////////////////////////////////////////////////////////////////////////
// UPixel2DTDTileMapActorFactory

UPixel2DTDTileMapActorFactory::UPixel2DTDTileMapActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("Pixel2DTopDown", "Pixel2DTileMapFactoryDisplayName", "Pixel2D TopDown Tile Map");
	NewActorClass = APixel2DTDTileMapActor::StaticClass();
}

void UPixel2DTDTileMapActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	APixel2DTDTileMapActor* TypedActor = CastChecked<APixel2DTDTileMapActor>(NewActor);
	UPixel2DTDTileMapComponent* RenderComponent = TypedActor->GetRenderComponent();
	check(RenderComponent);

	if (UPixel2DTDTileMap* TileMapAsset = Cast<UPixel2DTDTileMap>(Asset))
	{
		RenderComponent->UnregisterComponent();
		RenderComponent->TileMap = TileMapAsset;
		RenderComponent->SpawnFlipbooks();
		RenderComponent->RegisterComponent();
	}
	else if (RenderComponent->OwnsTileMap())
	{
		RenderComponent->UnregisterComponent();

		UPixel2DTDTileMap* OwnedTileMap = RenderComponent->GetTileMap();
		check(OwnedTileMap);

		GetDefault<UPixel2DTDImporterSettings>()->ApplySettingsForTileMapInit(OwnedTileMap, Cast<UPaperTileSet>(Asset));

		RenderComponent->RegisterComponent();
	}
}

void UPixel2DTDTileMapActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (APixel2DTDTileMapActor* TypedActor = Cast<APixel2DTDTileMapActor>(CDO))
	{
		UPixel2DTDTileMapComponent* RenderComponent = TypedActor->GetRenderComponent();
		check(RenderComponent);

		if (UPixel2DTDTileMap* TileMap = Cast<UPixel2DTDTileMap>(Asset))
		{
			RenderComponent->TileMap = TileMap;
			RenderComponent->SpawnFlipbooks();
		}
		else if (RenderComponent->OwnsTileMap())
		{
			UPixel2DTDTileMap* OwnedTileMap = RenderComponent->GetTileMap();
			check(OwnedTileMap);

			GetDefault<UPixel2DTDImporterSettings>()->ApplySettingsForTileMapInit(OwnedTileMap, Cast<UPaperTileSet>(Asset));
		}
	}
}

bool UPixel2DTDTileMapActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid())
	{
		UClass* AssetClass = AssetData.GetClass();
		if ((AssetClass != nullptr) && (AssetClass->IsChildOf(UPixel2DTDTileMap::StaticClass()) || AssetClass->IsChildOf(UPaperTileSet::StaticClass())))
		{
			return true;
		}
		else
		{
			OutErrorMsg = NSLOCTEXT("Pixel2DTopDown", "CanCreateActorFrom_NoTileMap", "No tile map was specified.");
			return false;
		}
	}
	else
	{
		return true;
	}
}
