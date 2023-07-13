// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
