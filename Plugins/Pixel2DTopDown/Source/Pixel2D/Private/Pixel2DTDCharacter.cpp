 //Polyart Studio 2021
 

#include "Pixel2DTDCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pixel2DTDComponent.h"

FName APixel2DTDCharacter::PixelComponentName(TEXT("Pixel0"));

//////////////////////////////////////////////////////////////////////////
// APixel2DTDCharacter

APixel2DTDCharacter::APixel2DTDCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	// Try to create the sprite component
	PixelComponent = CreateOptionalDefaultSubobject<UPixel2DTDComponent>(APixel2DTDCharacter::PixelComponentName);
	if (PixelComponent)
	{
		PixelComponent->AlwaysLoadOnClient = true;
		PixelComponent->AlwaysLoadOnServer = true;
		PixelComponent->bOwnerNoSee = false;
		PixelComponent->bAffectDynamicIndirectLighting = true;
		PixelComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		PixelComponent->SetupAttachment(GetCapsuleComponent());
		static FName CollisionProfileName(TEXT("CharacterAnimInstance"));
		PixelComponent->SetCollisionProfileName(CollisionProfileName);
		PixelComponent->SetGenerateOverlapEvents(false);
	}
}

void APixel2DTDCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!IsPendingKill())
	{
		if (PixelComponent)
		{
			// force animation tick after movement component updates
			if (PixelComponent->PrimaryComponentTick.bCanEverTick && GetCharacterMovement())
			{
				PixelComponent->PrimaryComponentTick.AddPrerequisite(GetCharacterMovement(), GetCharacterMovement()->PrimaryComponentTick);
			}
		}
	}
}
