// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Camera/GCameraDef.h"
#include "Level/Data/GLevelData.h"
#include "Character/Data/GHeroData.h"
#include "GConfigDataAsset.generated.h"

class UGAIItem;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class RUNNER_API UGConfigDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static UGConfigDataAsset* Get();
private:
	static TWeakObjectPtr<UGConfigDataAsset> s_Instance;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGLevelConfig> LevelConfigs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGCameraConfig> CameraConfigs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FGHeroConfig> HeroConfigs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UGAIItem>, TSoftObjectPtr<UBehaviorTree>> AIConfigs;
};
