// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/AbilityTask/GAT_WaitReceiveDamage.h"
#include "AbilitySystem/GAbilitySystemComponent.h"

UGAT_WaitReceiveDamage::UGAT_WaitReceiveDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TriggerOnce = false;
}

UGAT_WaitReceiveDamage* UGAT_WaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	UGAT_WaitReceiveDamage* MyObj = NewAbilityTask<UGAT_WaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UGAT_WaitReceiveDamage::Activate()
{
	UGAbilitySystemComponent* ASC = Cast<UGAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC)
	{
		ASC->ReceivedDamage.AddDynamic(this, &UGAT_WaitReceiveDamage::OnDamageReceived);
	}
}

void UGAT_WaitReceiveDamage::OnDestroy(bool AbilityIsEnding)
{
	UGAbilitySystemComponent* ASC = Cast<UGAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC)
	{
		ASC->ReceivedDamage.RemoveDynamic(this, &UGAT_WaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UGAT_WaitReceiveDamage::OnDamageReceived(UGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}
