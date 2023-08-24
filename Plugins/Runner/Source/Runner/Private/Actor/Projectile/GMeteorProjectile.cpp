// Copyright 2020 Dan Kestranek.


#include "Actor/Projectile/GMeteorProjectile.h"
#include "Character/GCharacter.h"
#include "AbilitySystem/GAbilitySystemComponent.h"


void AGMeteorProjectile::OnComponentOverlap(UPrimitiveComponent* OtherComponent)
{
	bool bIsWorldDynamic = OtherComponent->GetCollisionObjectType() == ECC_WorldDynamic;
	bool bIsWorldStatic = OtherComponent->GetCollisionObjectType() == ECC_WorldStatic;
	if(OtherComponent->GetCollisionObjectType() == ECC_WorldDynamic || OtherComponent->GetCollisionObjectType() == ECC_WorldStatic)
	{
		if(!HasAuthority())
		{
			Destroy();
			return;
		}

		TArray<AActor*> OverlappingActors;
		OtherComponent->GetOverlappingActors(OverlappingActors,AGCharacter::StaticClass());

		for(auto It = OverlappingActors.CreateIterator(); It; ++It)
		{
			AGCharacter* Character = Cast<AGCharacter>(*It);
			if(!Character->IsAlive())
			{
				continue;
			}
			if(Character == GetInstigator())
			{
				continue;
			}
			if(!Character->GetAbilitySystemComponent())
			{
				continue;
			}
			if (FGameplayEffectSpec* Spec = DamageEffectSpecHandle.Data.Get())
			{
				Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
			}

			if (FGameplayEffectSpec* Spec = StunEffectSpecHandle.Data.Get())
			{
				Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
			}
		}
	}

	Destroy();
}
