//Polyart Studio 2021


#include "Pixel2DTDActor.h"
#include "Pixel2DTDComponent.h"
#include "Pixel2DTDAnimInstance.h"
#include "PaperFlipbook.h"

//////////////////////////////////////////////////////////////////////////
// APixel2DTDActor

APixel2DTDActor::APixel2DTDActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RenderComponent = CreateDefaultSubobject<UPixel2DTDComponent>(TEXT("Pixel2DRenderComponent"));
	RootComponent = RenderComponent;
}

#if WITH_EDITOR
bool APixel2DTDActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);

	if (UPaperFlipbook* FlipbookAsset = RenderComponent->GetFlipbook())
	{
		Objects.Add(FlipbookAsset);
	}
	return true;
}
#endif

void APixel2DTDActor::RegisterAnimInstance(UClass * NewAnimInstance)
{
	RenderComponent->UnregisterComponent();
	RenderComponent->SetAnimInstance(NewAnimInstance);
	RenderComponent->RegisterComponent();
}
