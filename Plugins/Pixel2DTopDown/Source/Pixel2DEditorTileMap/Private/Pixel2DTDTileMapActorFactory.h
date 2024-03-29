//Polyart Studio 2021 


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "Pixel2DTDTileMapActorFactory.generated.h"

class AActor;
struct FAssetData;

UCLASS()
class UPixel2DTDTileMapActorFactory : public UActorFactory
{
	GENERATED_UCLASS_BODY()

	// UActorFactory interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	// End of UActorFactory interface
};
