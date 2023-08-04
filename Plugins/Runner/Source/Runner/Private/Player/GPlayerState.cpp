// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerState.h"

#include "AbilitySystem/GAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GAttributeSetBase.h"

AGPlayerState::AGPlayerState(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UGAttributeSetBase>(TEXT("AttributeSetBase"));
}

UAbilitySystemComponent* AGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGAttributeSetBase* AGPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void AGPlayerState::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddLambda([](const FOnAttributeChangeData& Data)
	{
		
	});
}
