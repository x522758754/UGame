// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GLevelSubsystem.generated.h"

class FGLevelLoadingBase;
/**
 * 
 */
UCLASS()
class RUNNER_API UGLevelSubsystem : public UGameInstanceSubsystem
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
public:
	const FString& GetOpenLevelName() const;
	void ChangeLevel(const FString& LevelId);
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

protected:
	void BeginLoading(ELoadingType Type, const FString& LevelId);
private:
	//Level
	bool OpenLevelLoaded = false;
	FString OpenLevelName;

	//Loading
	TSharedPtr<FGLevelLoadingBase> CurrentLoading;
	ELoadingType CurrentLoadingType;
};
