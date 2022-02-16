// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Pixel2DTDCharacter.h"
#include "GCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AGCharacterBase : public APixel2DTDCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
