// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "Event/GEventObserverHolder.h"
#include "GLevelSubsystem.generated.h"

class FGLevelLoadingBase;
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
		Dynamic = 0,
		Travel = 1,
		OpenLevel = 2,
		OpenLevel_Travel = 3,
		Transfer = 4,
		Num,
	};
	
	UFUNCTION()
	static UGLevelSubsystem* Get();
protected:
	static TWeakObjectPtr<UGLevelSubsystem> s_Instance;
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnGameInstanceInit() override;
	virtual void OnTick(float DeltaTime) override;
	
	void OnMapLoadComplete(const float LoadTime, const FString& MapName);
	void OnLevelLoaded();
public:
	int32 GetOpenLevelId() const;
	void ChangeLevel(int32 LevelId);
	
protected:
	void BeginLoading(ELoadingType Type, int32 LevelId);
private:
	//Level
	bool OpenLevelLoaded = false;
	int32 OpenLevelId;

	//Loading
	TSharedPtr<FGLevelLoadingBase> CurrentLoading;
	ELoadingType CurrentLoadingType;

	FGEventObserverHolder EventObserverHolder;
};
