// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacterInfoComponent.h"
#include "GHeroInfoComponent.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGHeroInfoComponent : public UGCharacterInfoComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGHeroInfoComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
