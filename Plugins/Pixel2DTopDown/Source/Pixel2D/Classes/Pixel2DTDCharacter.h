//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Character.h"
#include "Pixel2DTDCharacter.generated.h"

// APixel2DTDCharacter behaves like ACharacter, but uses a UPixel2DTDComponent instead of a USkeletalMeshComponent as a visual representation
// Note: The variable named Mesh will not be set up on this actor!
UCLASS()
class PIXEL2DTD_API APixel2DTDCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	// Name of the Sprite component
	static FName PixelComponentName;

private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPixel2DTDComponent* PixelComponent;
public:

	virtual void PostInitializeComponents() override;

	/** Returns Sprite subobject **/
	FORCEINLINE class UPixel2DTDComponent* GetSprite() const { return PixelComponent; }
};
