// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Character/Data/GNpcData.h"
#include "GNpcFunctions.generated.h"

/**
 *
 */
UCLASS()
class  UGNpcFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGNpcConfig DeepCopyNpcConfig(const FGNpcConfig& NpcConfig, UObject* Outer);
	static void SetNpcLabel(AActor* NpcActor, int NpcConfigId, int64 Guid);

public:
	static UGNpcInfoComponent* Client_SpawnNpc(const FGNpcConfig& NpcConfig, UWorld* World);
	static UGNpcInfoComponent* Client_SpawnNpcCharacter(const FGNpcConfig& NpcConfig, UWorld* World);

private:
	static AGNpcCharacter* SpawnNpcCharacter(const FGNpcConfig& NpcConfig, UWorld* World);
};
