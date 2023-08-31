// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacter.h"
#include "GHeroCharacter.generated.h"


/**
 * 
 */
UCLASS()
class RUNNER_API AGHeroCharacter : public AGCharacter
{
	GENERATED_BODY()

public:
	AGHeroCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	void BindAscInput();
	
};
