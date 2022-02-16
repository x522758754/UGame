// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GPlayerState.generated.h"

class UGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RUNNER_API AGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGPlayerState(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	UPROPERTY()
	UGAbilitySystemComponent* AbilitySystemComponent;
};