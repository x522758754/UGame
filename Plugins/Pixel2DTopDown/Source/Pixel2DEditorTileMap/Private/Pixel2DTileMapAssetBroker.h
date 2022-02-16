//Polyart Studio 2021 


#pragma once

#include "CoreMinimal.h"
#include "Pixel2DTDTileMap.h"
#include "ComponentAssetBroker.h"
#include "Pixel2DTDTileMapComponent.h"

//////////////////////////////////////////////////////////////////////////
// FPixel2DTileMapAssetBroker

class FPixel2DTileMapAssetBroker : public IComponentAssetBroker
{
public:
	UClass* GetSupportedAssetClass() override
	{
		return UPixel2DTDTileMap::StaticClass();
	}

	virtual bool AssignAssetToComponent(UActorComponent* InComponent, UObject* InAsset) override
	{
		if (UPixel2DTDTileMapComponent* RenderComp = Cast<UPixel2DTDTileMapComponent>(InComponent))
		{
			UPixel2DTDTileMap* TileMap = Cast<UPixel2DTDTileMap>(InAsset);

			if ((TileMap != nullptr) || (InAsset == nullptr))
			{
				RenderComp->TileMap = TileMap;
				return true;
			}
		}

		return false;
	}

	virtual UObject* GetAssetFromComponent(UActorComponent* InComponent) override
	{
		if (UPixel2DTDTileMapComponent* RenderComp = Cast<UPixel2DTDTileMapComponent>(InComponent))
		{
			if ((RenderComp->TileMap != nullptr) && (RenderComp->TileMap->IsAsset()))
			{
				return RenderComp->TileMap;
			}
		}

		return nullptr;
	}
};

