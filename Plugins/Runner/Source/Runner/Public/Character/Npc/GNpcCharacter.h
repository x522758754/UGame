// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacter.h"
#include "Character/Data/GNpcData.h"
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
	
	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void ApplyNpcConfig(const FGNpcConfig& InNpcConfig);
	
	void SpawnController();
protected:

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle NpcConfigApplayedDelegateHandle;

	UPROPERTY()
	UGAbilitySystemComponent* HardRefAbilitySystemComponent;

	UPROPERTY()
	UGAttributeSetBase* HardRefAttributeSetBase;
};
