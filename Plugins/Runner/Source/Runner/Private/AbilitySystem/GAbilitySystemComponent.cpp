// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAbilitySystemComponent.h"

void UGAbilitySystemComponent::ReceiveDamage(UGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
