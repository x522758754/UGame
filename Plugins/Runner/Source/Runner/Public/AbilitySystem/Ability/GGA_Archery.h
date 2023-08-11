// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterDef.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "GGA_Archery.generated.h"

enum class EGAttackType : uint8;
class AGProjectile;
/**
 * 射箭，发出弓箭
 */
UCLASS()
class UGGA_Archery : public UGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGGA_Archery();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGAttackType AttackType = EGAttackType::RangedAttack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<AGProjectile> ProjectileClassPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffectClassPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag TagEvent_MontageSpawnProjectile;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag TagEvent_MontageEndAbility;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
