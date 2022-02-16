//Polyart Studio 2021
 

#include "Pixel2DTDAnimBlueprint.h"
#include "Pixel2DTDAnimBlueprintGeneratedClass.h"

UPixel2DTDAnimBlueprint::UPixel2DTDAnimBlueprint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseMultiThreadedAnimationUpdate = true;
}

void UPixel2DTDAnimBlueprint::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

#if WITH_EDITOR

UClass* UPixel2DTDAnimBlueprint::GetBlueprintClass() const
{
	return UPixel2DTDAnimBlueprintGeneratedClass::StaticClass();
}

bool UPixel2DTDAnimBlueprint::CanRecompileWhilePlayingInEditor() const
{
	return true;
}

void UPixel2DTDAnimBlueprint::PostLoad()
{
	Super::PostLoad();
}

#endif