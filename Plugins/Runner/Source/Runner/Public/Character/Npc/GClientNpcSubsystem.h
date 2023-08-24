// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Data/GNpcData.h"
#include "System/GGameInstanceSubsystem.h"
#include "Event/GEventObserverHolder.h"
#include "GClientNpcSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGClientNpcSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static UGClientNpcSubsystem* Get();
protected:
	static TWeakObjectPtr<UGClientNpcSubsystem> s_Instance;

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnGameInstanceInit() override;
	virtual void OnTick(float DeltaTime) override;

public:
	int64 CreateNpc(const FGNpcConfig& NpcConfig, bool bStandaloneServer,
		const TFunction<void(const FGClientNpcInfo&)>& AfterNpcSpawnFunction = nullptr, FVector* Location = nullptr);
	void DestroyNpcByGuid(int64 Guid);
	void DestroyNpcByConfigId(int NpcConfigId);
	FGClientNpcInfo* GetClientNpcInfo(int64 Guid);

protected:
	void OnMapLoadComplete(int SceneId, float LoadTime, const FString& MapName);
	void OnWorldBeginTearDown(UWorld* InWorld);

protected:
	TMap<int64, FGClientNpcInfo> ClientNpcInfoMap;
	FGEventObserverHolder EventObserverHolder;
};
