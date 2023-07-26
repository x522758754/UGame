// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GGameInstanceSubsystem.generated.h"

class UGGameInstance;
/**
 * 
 */
UCLASS()
class RUNNER_API UGGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnGameInstanceInit() {}
	virtual void OnTick(float DeltaTime) {}
};

