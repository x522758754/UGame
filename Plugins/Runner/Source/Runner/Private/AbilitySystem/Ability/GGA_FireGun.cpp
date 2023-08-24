// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/Ability/GGA_FireGun.h"

#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "AbilitySystem/AbilityTask/GAT_PlayMontageAndWaitForEvent.h"
#include "Character/GCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Projectile/GProjectile.h"

UGGA_FireGun::UGGA_FireGun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGGA_FireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	UAnimMontage* MontageToPlay = FireHipMontage;
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGGameplayTags::Get().State_AimDownSights) &&
		!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights.Removal"))))
	{
		MontageToPlay = FireIronsightsMontage;
	}

	if(MontageToPlay)
	{
		// Play fire montage and wait for event telling us to spawn the projectile
		UGAT_PlayMontageAndWaitForEvent* Task = UGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &UGGA_FireGun::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGGA_FireGun::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGGA_FireGun::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGGA_FireGun::OnCancelled);
		Task->EventReceived.AddDynamic(this, &UGGA_FireGun::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGGA_FireGun::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_FireGun::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_FireGun::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGGameplayTags::Get().Event_MontageEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		AGCharacter* Hero = Cast<AGCharacter>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		// FVector Start = Hero->GetGunComponent()->GetSocketLocation(FName("Muzzle"));
		// FVector End = Hero->GetCameraBoom()->GetComponentLocation() + Hero->GetFollowCamera()->GetForwardVector() * Range;
		FVector Start = Hero->GetActorLocation();
		FVector End = Hero->GetActorLocation() + Hero->GetActorForwardVector() * Range;
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		
		// Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

		//FTransform MuzzleTransform = Hero->GetGunComponent()->GetSocketTransform(FName("Muzzle"));
		FTransform MuzzleTransform = Hero->GetActorTransform();
		MuzzleTransform.SetRotation(Rotation.Quaternion());
		MuzzleTransform.SetScale3D(FVector(1.0f));

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGProjectile>(ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),
			Hero, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Projectile->Range = Range;
		Projectile->FinishSpawning(MuzzleTransform);
	}
}
