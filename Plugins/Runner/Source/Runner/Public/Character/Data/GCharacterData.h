// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCharacterData.generated.h"

USTRUCT()
struct FGCharacterCommonMoveAttr
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float MaxStepHeight = 0.5f;
};
