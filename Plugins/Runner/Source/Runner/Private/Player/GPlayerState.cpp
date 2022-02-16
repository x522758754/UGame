// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerState.h"

#include "Characters/Abilities/GAbilitySystemComponent.h"

AGPlayerState::AGPlayerState(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
