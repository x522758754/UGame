// Copyright 2020 Dan Kestranek.


#include "Actor/Projectile/GProjectile.h"

#include "AbilitySystemComponent.h"
#include "Character/GCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGProjectile::AGProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	
}

void AGProjectile::OnOverlap(AActor* OtherActor)
{
	if(GetInstigator() == OtherActor)
	{
		return;
	}
	
	if(!HasAuthority())
	{
		Destroy();
		return;
	}

	if(AGCharacter* Character = Cast<AGCharacter>(OtherActor))
	{
		TArray<FHitResult> OutHits;
		//const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic), UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)};
		if(GetWorld()->LineTraceMultiByObjectType(OutHits, GetActorLocation(), Character->GetActorLocation(), FCollisionObjectQueryParams(ECC_Pawn)))
		{
			for(auto &HitResult : OutHits)
			{
				if(HitResult.Actor == OtherActor)
				{
					if (FGameplayEffectSpec* Spec = DamageEffectSpecHandle.Data.Get())
					{
						FGameplayEffectContextHandle EffectContext = Spec->GetEffectContext();
						EffectContext.AddHitResult(HitResult);
						
						Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
						Destroy();
						break;
					}
				}
			}

		}
	}
}

// Called when the game starts or when spawned
void AGProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileMovement->InitialSpeed > 0)
	{
		float TimeSpan = Range / ProjectileMovement->InitialSpeed;
		SetLifeSpan(TimeSpan);
	}
}