// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GCommonFunctions.generated.h"

class AGPlayerCameraManager;
class AGPlayerController;
class AGHeroCharacter;
/**
 *
 */
UCLASS()
class  UGCommonFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UWorld* GetCurWorld();

	static AGPlayerController* GetPlayerController();

	static AGPlayerCameraManager* GetPlayerCameraManager();
};



