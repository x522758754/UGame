// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Camera/GCameraDef.h"
#include "Level/Data/GLevelData.h"
#include "Character/Data/GHeroData.h"
#include "GConfigData.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static UGConfigData* Get();
private:
	static TWeakObjectPtr<UGConfigData> s_Instance;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGLevelConfig> LevelConfigs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGCameraConfig> CameraConfigs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGHeroConfig> HeroConfigs;
};
