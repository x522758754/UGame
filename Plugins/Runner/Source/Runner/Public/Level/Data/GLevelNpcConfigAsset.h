// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/DataAsset.h"
#include "Character/Data/GNpcData.h"
#include "GLevelNpcConfigAsset.generated.h"

UCLASS()
class RUNNER_API UGLevelNpcConfigAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, FGNpcConfig> NpcConfigMap;
};

