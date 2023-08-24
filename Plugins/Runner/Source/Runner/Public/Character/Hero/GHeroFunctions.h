// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GHeroFunctions.generated.h"

class AGHeroCharacter;
/**
 *
 */
UCLASS()
class  UGHeroFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static AGHeroCharacter* SpawnHero(int32 ConfigId, const FTransform& Transform);
};



