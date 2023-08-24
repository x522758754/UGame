// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/Ability/GGA_PassiveArmor.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectStackChange.h"
#include "System/GAssetManager.h"
#include "AbilitySystem/AbilityTask/GAT_WaitReceiveDamage.h"
#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


UGGA_PassiveArmor::UGGA_PassiveArmor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGGA_PassiveArmor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OnWait();

	WaitReceiveDamageTask = UGAT_WaitReceiveDamage::WaitReceiveDamage(this, false);
	WaitReceiveDamageTask->OnDamage.AddDynamic(this, &UGGA_PassiveArmor::OnDamage);
}

void UGGA_PassiveArmor::OnWait()
{
	RechargeDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, RechargeDelay);
	RechargeDelayTask->OnFinish.AddDynamic(this, &UGGA_PassiveArmor::OnFinish);
	ShouldLoopWaitDelayTask = true;	
}


void UGGA_PassiveArmor::OnStackChange()
{
	StackChangeTask = UAbilityTask_WaitGameplayEffectStackChange::WaitForGameplayEffectStackChange(this, GE_StackGeneratorHandle);
	StackChangeTask->OnChange.AddDynamic(this, &UGGA_PassiveArmor::OnGameplayEffectStackChange);
}

void UGGA_PassiveArmor::OnFinish()
{
	if(TSubclassOf<UGameplayEffect> GEArmorClass = UGAssetManager::Get()->LoadSubclass(GEArmorClassPath))
	{
		FGameplayEffectSpecHandle GEArmorSpecHandle = MakeOutgoingGameplayEffectSpec(GEArmorClass, GetAbilityLevel());
		GE_StackGeneratorHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GEArmorSpecHandle);

		OnStackChange();

		if(ShouldLoopWaitDelayTask)
		{
			OnWait();
		}
	}
}

void UGGA_PassiveArmor::OnDamage(UGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
	{
		return;
	}

	UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (AbilitySystemComponent)
	{
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(WithGrantedTags);
		AbilitySystemComponent->RemoveActiveEffects(Query, 1);
	}
}

void UGGA_PassiveArmor::OnGameplayEffectStackChange(FActiveGameplayEffectHandle Handle, int32 NewCount, int32 OldCount)
{
	if(MaxStacks == NewCount)
	{
		if(RechargeDelayTask)
		{
			RechargeDelayTask->EndTask();
			RechargeDelayTask = nullptr;
			ShouldLoopWaitDelayTask = false;
		}
	}
	else if(MaxStacks == OldCount)
	{
		OnWait();
	}
}

