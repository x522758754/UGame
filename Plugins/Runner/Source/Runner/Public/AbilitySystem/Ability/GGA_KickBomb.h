// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterDef.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "GGA_KickBomb.generated.h"

enum class EGAttackType : uint8;
class AGProjectile;
/**
 * 踢炸弹， 持续按方向键多久才生效
 */
UCLASS()
class UGGA_KickBomb : public UGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGGA_KickBomb() {};

// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	EGAttackType AttackType = EGAttackType::RangedAttack;
//
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	TSoftClassPtr<AGProjectile> ProjectileClassPath;
//
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	TSoftClassPtr<UGameplayEffect> DamageGameplayEffectClassPath;
//
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	FGameplayTag TagEvent_MontageSpawnProjectile;
//
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	FGameplayTag TagEvent_MontageEndAbility;
//
// 	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
// 	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
//
// protected:
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	float Range = 1000;
//
// 	UPROPERTY(BlueprintReadOnly, EditAnywhere)
// 	float Damage = 1.f;
//
// 	UFUNCTION()
// 	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
//
// 	UFUNCTION()
// 	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
//
// 	UFUNCTION()
// 	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
