// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GGameConfigSettings.generated.h"

class UGLevelData;

/**
 * 
 */
UCLASS(Config=GameConfigSettings, DefaultConfig,configdonotcheckdefaults)
class RUNNER_API UGGameConfigSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UGGameConfigSettings* Get()
	{
		return GetDefault<UGGameConfigSettings>();
	}

	virtual FName GetCategoryName() const override{return "Game";}
public:

	UPROPERTY(config, noclear, EditAnywhere)
	TSoftObjectPtr<UGLevelData> InitLevelData;
};
