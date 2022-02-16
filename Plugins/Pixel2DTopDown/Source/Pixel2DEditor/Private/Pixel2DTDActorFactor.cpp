//Polyart Studio 2021


#include "Pixel2DTDActorFactory.h"
#include "AssetData.h"
#include "Pixel2DTDActor.h"
#include "Pixel2DTDComponent.h"
#include "Pixel2DTDAnimBlueprint.h"

//////////////////////////////////////////////////////////////////////////
// UPixel2DTDActorFactory

UPixel2DTDActorFactory::UPixel2DTDActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("SpriteAnim", "SpriteFactoryDisplayName", "Add SpriteAnim");
	NewActorClass = APixel2DTDActor::StaticClass();
}

void UPixel2DTDActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	if (UPixel2DTDAnimBlueprint* SpriteAnim = Cast<UPixel2DTDAnimBlueprint>(Asset))
	{
		APixel2DTDActor* TypedActor = CastChecked<APixel2DTDActor>(NewActor);
		TypedActor->RegisterAnimInstance(SpriteAnim->GeneratedClass);
	}
}

void UPixel2DTDActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (UPixel2DTDAnimBlueprint* SpriteAnim = Cast<UPixel2DTDAnimBlueprint>(Asset))
	{
		if (APixel2DTDActor* TypedActor = Cast<APixel2DTDActor>(CDO))
		{
			TypedActor->RegisterAnimInstance(SpriteAnim->GeneratedClass);
		}
	}
}

bool UPixel2DTDActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid() && AssetData.GetClass()->IsChildOf(UPixel2DTDAnimBlueprint::StaticClass()))
	{
		return true;
	}
	else
	{
		OutErrorMsg = NSLOCTEXT("SpriteAnim", "CanCreateActorFrom_NoSprite", "No sprite was specified.");
		return false;
	}
}
