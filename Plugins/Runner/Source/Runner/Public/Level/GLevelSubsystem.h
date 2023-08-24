// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "Event/GEventObserverHolder.h"
#include "GLevelSubsystem.generated.h"

struct FGLevelConfig;
class FGLevelLoadingBase;
class UGLevelNpcConfigAsset;
class AGLevelActor;
/**
 * 
 */
UCLASS()
class RUNNER_API UGLevelSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	enum class ELoadingType : int8
	{
		OpenLevel = 1,
		Num,
	};
	
	UFUNCTION()
	static UGLevelSubsystem* Get();
protected:
	static TWeakObjectPtr<UGLevelSubsystem> s_Instance;

public:
	int32 GetOpenLevelId() const;
	void ChangeLevel(int32 LevelId);
	UGLevelNpcConfigAsset* GetCurrentLevelNpcConfigAsset() const;

	bool TryInitLevel();
	bool TryPreparePlayer();
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnGameInstanceInit() override;
	virtual void OnTick(float DeltaTime) override;
	
	void OnLevelBeginLoading(ELoadingType Type, int32 LevelId);
	void OnMapLoadComplete(int32 LevelId, const float LoadTime, const FString& MapName);
	void OnLevelLoaded();
private:
	//Level
	int32 OpenLevelId;

	const FGLevelConfig* CurrentLevelConfig = nullptr;
	UPROPERTY(Transient)
	UGLevelNpcConfigAsset* CurrentLevelNpcConfigAsset;
	UPROPERTY(Transient)
	TWeakObjectPtr<AGLevelActor> LevelActor;

	//Loading
	TSharedPtr<FGLevelLoadingBase> CurrentLoading;
	ELoadingType CurrentLoadingType;

	FGEventObserverHolder EventObserverHolder;
};
