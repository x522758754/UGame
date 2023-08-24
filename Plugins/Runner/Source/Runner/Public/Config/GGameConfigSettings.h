// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/GCameraDef.h"
#include "Engine/DeveloperSettings.h"
#include "Config/GConfigDataAsset.h"
#include "GGameConfigSettings.generated.h"

class AGHeroCharacter;
class AGCameraActor;

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

	UPROPERTY(Config, noclear, EditAnywhere)
	TSoftObjectPtr<UGConfigDataAsset> ConfigDataPath;

	UPROPERTY(Config, noclear, EditAnywhere)
	TSoftClassPtr<AGCameraActor> Camera;
	
	UPROPERTY(config, noclear, EditAnywhere)
	int32 DefaultLevelId = 1;
	
	UPROPERTY(config)
	int32 DefaultCameraId = 1;

	UPROPERTY(config, noclear, EditAnywhere)
	FGCameraParam DefaultCameraParam;

	UPROPERTY(config, noclear, EditAnywhere)
	int32 DefaultHeroId = 1;

	UPROPERTY(config, EditAnywhere, Category = "Input", meta = (ClampMin = 0.f))
	float InputClickMaxDistance = 2;

	UPROPERTY(config, noclear, EditAnywhere)
	float ClientNpcLoadDistance = 1000;

	UPROPERTY(config, noclear, EditAnywhere)
	float ClientNpcDisappearDistance = 2000;
};
