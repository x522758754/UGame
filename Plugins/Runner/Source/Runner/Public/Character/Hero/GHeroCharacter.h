// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacterBase.h"
#include "GHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AGHeroCharacter : public AGCharacterBase
{
	GENERATED_BODY()

public:
	AGHeroCharacter(const class FObjectInitializer& ObjectInitializer);
};
