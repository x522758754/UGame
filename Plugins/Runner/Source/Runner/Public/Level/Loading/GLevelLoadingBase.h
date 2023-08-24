// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class RUNNER_API FGLevelLoadingBase
{
	friend class UGLevelSubsystem;
public:
	FGLevelLoadingBase(int32 MaxStage);
	virtual ~FGLevelLoadingBase() {}
public:
	virtual void OnTick(float DeltaTime);
public:
	virtual void Begin(int32 LevelId);
	virtual void LoadEnd();
	float GetProgress() const { return Progress; }

	int32 GetLoadingLevelId() const {return LoadingLevelId;}
protected:
	void SetCurrentStage(int Stage);
	void SetNextStage();
	virtual void OnMapLoaded() {};
protected:
	bool IsStreamingCompleted(const FVector& Location);
public:
	float PredictTime = 6;				//Seconds;
	float PredictFallBehindTime = 2;	//Seconds;进度落后时
protected:
	float Progress = 0;
	float MaxDeltaProgress = 0.1;

protected:
	int32 MaxStage;
	TArray<float> ProgressStage;
	int CurrentStage = 0;
	float CurrentStageProgress = 0;
	float PrevStateProgress = 0;

	int32 LoadingLevelId;
};
