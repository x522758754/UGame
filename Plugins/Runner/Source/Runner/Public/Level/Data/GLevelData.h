// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"

#include "GLevelData.generated.h"


/** Description of settings used to display experiences in the UI and start a new session */
UCLASS(BlueprintType)
class RUNNER_API UGLevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FString GetMapName() const;

	virtual FString ConstructTravelURL() const;

public:
	/** The specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowedClasses="/Script/Engine.World"))
	FSoftObjectPath MapLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, FString> ExtraArgs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowedClasses="/Script/Engine.World"))
	FSoftObjectPath NextMapLevel;
};


