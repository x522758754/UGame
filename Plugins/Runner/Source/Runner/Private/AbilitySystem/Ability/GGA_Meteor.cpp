// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/Ability/GGA_Meteor.h"

#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "System/GAssetManager.h"
#include "AbilitySystem/AbilityTask/GAT_AttackAndWaitForEvent.h"
#include "Character/Hero/GHeroCharacter.h"
#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Projectile/GMeteorProjectile.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"


UGGA_Meteor::UGGA_Meteor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGGA_Meteor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TSubclassOf<AGameplayAbilityTargetActor_GroundTrace> TargetActorClass = UGAssetManager::LoadSubclass(TargetActorClassPath);
	if(TargetActorClass == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitTargetData* WaitTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, TargetActorClass);
	WaitTask->ValidData.AddDynamic(this, &UGGA_Meteor::OnValidData);
	WaitTask->Cancelled.AddDynamic(this, &UGGA_Meteor::OnCancelled);

	AGameplayAbilityTargetActor* SpawnedActor = nullptr;
	if(WaitTask->BeginSpawningActor(this, TargetActorClass, SpawnedActor))
	{
		if(AGameplayAbilityTargetActor_GroundTrace* TargetActor = Cast<AGameplayAbilityTargetActor_GroundTrace>(SpawnedActor))
		{
			TargetActor->MaxRange = Range;
			TargetActor->TraceProfile = TraceProfile;
			TargetActor->bTraceAffectsAimPitch = true;

			FGameplayAbilityTargetingLocationInfo LocationInfo = MakeTargetLocationInfoFromOwnerActor();

			AGHeroCharacter* Hero = Cast<AGHeroCharacter>(GetAvatarActorFromActorInfo());
			if (Hero)
			{
				LocationInfo.LiteralTransform.SetLocation(Hero->GetMoveDirection() * 200 + LocationInfo.LiteralTransform.GetLocation());
				TargetActor->StartLocation = MoveTemp(LocationInfo);


				WaitTask->FinishSpawningActor(this, SpawnedActor);
				WaitTask->ReadyForActivation();
				
				return;
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_Meteor::OnCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGGA_Meteor::OnValidData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AGHeroCharacter* Hero = Cast<AGHeroCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero->HasAuthority())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	int32 Index = 0;
	if(TargetData.Data.IsValidIndex(Index))
	{
		FGameplayAbilityTargetData* Data = TargetData.Data[Index].Get();
		if (Data)
		{
			const FHitResult* HitResultPtr = Data->GetHitResult();
			if (HitResultPtr)
			{
				if(TSubclassOf<AGProjectile> RockClass = UGAssetManager::Get()->LoadSubclass(RockClassPath))
				{
					FTransform SpawnTransform = Hero->GetActorTransform();
					SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
					SpawnTransform.SetScale3D(FVector(1.0f));
					SpawnTransform.SetLocation(HitResultPtr->Location + FVector(0.0f, 0.0f, 500.0f));

					FActorSpawnParameters SpawnParameters;
					SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					AGMeteorProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGMeteorProjectile>(RockClass, SpawnTransform, GetOwningActorFromActorInfo(),Hero, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if(Projectile)
					{
						if(TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = UGAssetManager::Get()->LoadSubclass(DamageGameplayEffectClassPath))
						{
							FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffectClass, GetAbilityLevel());
		
							// Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
							if(DamageEffectSpecHandle.IsValid())
							{
								DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGGameplayTags::Get().Data_Damage, Damage);
							}

							Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
						}
						
						if(TSubclassOf<UGameplayEffect> StunGameplayEffectClass = UGAssetManager::Get()->LoadSubclass(StunGameplayEffectClassPath))
						{
							FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(StunGameplayEffectClass, GetAbilityLevel());
							Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
						}

						Projectile->FinishSpawning(SpawnTransform);
						EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
						return;
					}
				}
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}