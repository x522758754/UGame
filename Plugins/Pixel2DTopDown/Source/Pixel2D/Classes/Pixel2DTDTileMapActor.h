//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "PaperTileMapActor.h"
#include "Pixel2DTDTileMapComponent.h"
#include "Pixel2DTDTileMapActor.generated.h"

/**
* Implements Pixel2DTDTileMapActor
*/

UCLASS(ComponentWrapperClass, HideCategories = ("Tile Map"))
class PIXEL2DTD_API APixel2DTDTileMapActor : public AActor//APaperTileMapActor
{
//	GENERATED_UCLASS_BODY()
//
//public:
//	FORCEINLINE class UPixel2DTDTileMapComponent* GetRenderComponent() const
//	{
//		return CastChecked<class UPixel2DTDTileMapComponent>(APaperTileMapActor::GetRenderComponent());
//	}

	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(Category = PixelTileMapActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	class UPixel2DTDTileMapComponent* RenderComponent;
public:

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPixel2DTDTileMapComponent* GetRenderComponent() const { return RenderComponent; }

};
