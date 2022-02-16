//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDComponent.h"
#include "Pixel2DTDActor.generated.h"

/**
* An instance of a UPixel2DTDAnimInstance in a level.
*
* This actor is created when you drag a UPixel2DTDAnimInstance asset from the content browser into the level, and
* it is just a thin wrapper around a UPixel2DTDComponent that actually references the asset.
*/
UCLASS(ComponentWrapperClass, Blueprintable)
class PIXEL2DTD_API APixel2DTDActor : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
	class UPixel2DTDComponent* RenderComponent;

public:

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	void RegisterAnimInstance(UClass * NewAnimInstance);

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPixel2DTDComponent*     GetRenderComponent() const { return RenderComponent; }
};


