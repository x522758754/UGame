// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLevelData.generated.h"

USTRUCT(BlueprintType)
struct RUNNER_API FGLevelConfig
{
	GENERATED_BODY()
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowedClasses="/Script/Engine.World"))
	FSoftObjectPath MapLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, FString> ExtraArgs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowedClasses="/Script/Engine.World"))
	FSoftObjectPath NextMapLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform BornTransform;

public:
	FString GetMapName() const;

	FString ConstructTravelURL() const;
};

