// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Loading/GLevelLoadingBase.h"

#include "Event/GEventBasic.h"

FGLevelLoadingBase::FGLevelLoadingBase(int32 MaxStage)
{
	ProgressStage.Empty();

	for(int32 i = 0; i < MaxStage; ++i)
	{
		ProgressStage.Add(i / (float)MaxStage);
	}
}

void FGLevelLoadingBase::OnTick(float DeltaTime)
{
	if (Progress >= 1)
	{
		return;
	}
	float Delta = 0;
	if(Progress < PrevStateProgress)
	{
		Delta = DeltaTime / PredictFallBehindTime;
	}
	else
	{
		Delta = DeltaTime / PredictTime;
	}
	if (Delta > MaxDeltaProgress)
	{
		Delta = MaxDeltaProgress;
	}
	Progress += Delta;
	if (Progress > CurrentStageProgress)
	{
		Progress = CurrentStageProgress;
	}

	if(Progress == 1 && FUObjectThreadContext::Get().IsRoutingPostLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("FGLevelLoading::OnTick, FUObjectThreadContext::Get().IsRoutingPostLoad"))
		Progress = 0.999f;
	}
}

void FGLevelLoadingBase::SetCurrentStage(int Stage)
{
	if(CurrentStage != Stage)
	{
		UE_LOG(LogTemp, Log, TEXT("FGLevelLoading::SetCurrentStage %d-%d"), Stage, ProgressStage.Num());
	}
	CurrentStage = Stage;
	if (CurrentStage >= 0 && CurrentStage < ProgressStage.Num())
	{
		CurrentStageProgress = ProgressStage[CurrentStage];
	}
	else
	{
		CurrentStageProgress = 1;
	}
	if(CurrentStage > 0)
	{
		PrevStateProgress = ProgressStage[CurrentStage - 1];
	}
	else
	{
		PrevStateProgress = 0;
	}
}

void FGLevelLoadingBase::SetNextStage()
{
	SetCurrentStage(CurrentStage + 1);
}

void FGLevelLoadingBase::Begin(int32 LevelId)
{
	LoadingLevelId = LevelId;
	
	SetCurrentStage(0);
	Progress = 0;
}

void FGLevelLoadingBase::LoadEnd()
{
	UE_LOG(LogTemp, Log, TEXT("FGLevelLoading::LoadEnd"))
}

bool FGLevelLoadingBase::IsStreamingCompleted(const FVector& Location)
{
	return true;
}
