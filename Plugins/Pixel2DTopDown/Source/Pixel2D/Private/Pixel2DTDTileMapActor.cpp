//Polyart Studio 2021


#include "Pixel2DTDTileMapActor.h"
#include "Pixel2DTDTileMapComponent.h"

#define LOCTEXT_NAMESPACE "Pixel2DTopDown"

//////////////////////////////////////////////////////////////////////////
// APixel2DTDTileMapActor

APixel2DTDTileMapActor::APixel2DTDTileMapActor(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	RenderComponent = CreateDefaultSubobject<UPixel2DTDTileMapComponent>(TEXT("RenderComponent"));

	RootComponent = RenderComponent;
}

#if WITH_EDITOR
bool APixel2DTDTileMapActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	AActor::GetReferencedContentObjects(Objects);

	if (const UObject* Asset = RenderComponent->AdditionalStatObject())
	{
		Objects.Add(const_cast<UObject*>(Asset));
	}
	return true;
}
#endif

#undef LOCTEXT_NAMESPACE