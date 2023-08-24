// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacter.h"
#include "GameplayEffectTypes.h"
#include "GNpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AGNpcCharacter : public AGCharacter
{
	GENERATED_BODY()

public:
	AGNpcCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void RefreshNpcConfig();

protected:
	virtual void BeginPlay() override;
	
	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
protected:

	UPROPERTY()
	UGAbilitySystemComponent* HardRefAbilitySystemComponent;

	UPROPERTY()
	UGAttributeSetBase* HardRefAttributeSetBase;
};
