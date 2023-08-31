// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GCommonFunctions.generated.h"

enum class EGDirection : uint8;
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

	UFUNCTION(BlueprintCallable)
	static AGPlayerController* GetPlayerController();
	UFUNCTION(BlueprintCallable)
	static AGHeroCharacter* GetHeroCharacter();
	UFUNCTION(BlueprintCallable)
	static AGPlayerCameraManager* GetPlayerCameraManager();
	UFUNCTION(BlueprintCallable)
	static EGDirection GetDirection(FVector From, FVector To);
	UFUNCTION(BlueprintCallable)
	static FVector GetDirectionVector(EGDirection Direction);
};



