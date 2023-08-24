// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHeroData.generated.h"

class AGHeroCharacter;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FGHeroConfig
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	int32 ConfigId;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero")
	TSoftClassPtr<AGHeroCharacter> HeroClass;
};