// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterDef.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "GGA_PassiveArmor.generated.h"

class UAbilityTask_WaitDelay;
class UAbilityTask_WaitGameplayEffectStackChange;
class UGAT_WaitReceiveDamage;
/**
 * 被动护盾叠加 每过4s角色获得一个最大层数为4的护盾, 每次受到伤害时移除一层护盾.
 */
UCLASS()
class UGGA_PassiveArmor : public UGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGGA_PassiveArmor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float RechargeDelay = 4.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxStacks = 4;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer WithGrantedTags;

	TSoftClassPtr<UGameplayEffect> GEArmorClassPath;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	void OnWait();

	void OnStackChange();

	UFUNCTION()
	void OnFinish();

	UFUNCTION()
	void OnDamage(class UGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	UFUNCTION()
	void OnGameplayEffectStackChange(FActiveGameplayEffectHandle Handle, int32 NewCount, int32 OldCount);


protected:
	UPROPERTY()
	UAbilityTask_WaitDelay* RechargeDelayTask;

	UPROPERTY()
	UGAT_WaitReceiveDamage* WaitReceiveDamageTask;

	UPROPERTY()
	UAbilityTask_WaitGameplayEffectStackChange* StackChangeTask;
	
	bool ShouldLoopWaitDelayTask = false;

	FActiveGameplayEffectHandle GE_StackGeneratorHandle;
	
};
