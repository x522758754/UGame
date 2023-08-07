// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/Ability/GGA_Archery.h"

#include "GAT_AttackAndWaitForEvent.h"
#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "AbilitySystem/AbilityTask/GAT_PlayMontageAndWaitForEvent.h"
#include "Character/GCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/GProjectile.h"


UGGA_Archery::UGGA_Archery()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGGA_Archery::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	// Play fire montage and wait for event telling us to spawn the projectile
	UGAT_AttackAndWaitForEvent* Task = UGAT_AttackAndWaitForEvent::AttackAndWaitForEvent(this, EGAttackType::RangedAttack, NAME_None, FGameplayTagContainer());
	Task->OnCompleted.AddDynamic(this, &UGGA_Archery::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGGA_Archery::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGGA_Archery::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGGA_Archery::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UGGA_Archery::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_Archery::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_Archery::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == TagEvent_MontageEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == TagEvent_MontageSpawnProjectile)
	{
		AGCharacterBase* Hero = Cast<AGCharacterBase>(GetAvatarActorFromActorInfo());
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
		//DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

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
