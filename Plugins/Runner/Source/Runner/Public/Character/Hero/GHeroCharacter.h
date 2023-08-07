// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacterBase.h"
#include "Character/CharacterDef.h"
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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	void BindAscInput();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isMoving = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGMoveDirection MoveDirection = EGMoveDirection::Right;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isAttacking = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGAttackType AttackType = EGAttackType::None;
	
};
