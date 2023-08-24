// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterDef.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "GGA_Meteor.generated.h"

class AGameplayAbilityTargetActor_GroundTrace;
enum class EGAttackType : uint8;
class AGProjectile;
/**
 * 流星
 */
UCLASS()
class UGGA_Meteor : public UGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGGA_Meteor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGAttackType AttackType = EGAttackType::MagicAttack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<AGameplayAbilityTargetActor_GroundTrace> TargetActorClassPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<AGProjectile> RockClassPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffectClassPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<UGameplayEffect> StunGameplayEffectClassPath;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range = 1000;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCollisionProfileName TraceProfile = FName(TEXT("TargetActorGroundLocation"));

	UFUNCTION()
	void OnCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnValidData(const FGameplayAbilityTargetDataHandle& TargetData);
	
};
