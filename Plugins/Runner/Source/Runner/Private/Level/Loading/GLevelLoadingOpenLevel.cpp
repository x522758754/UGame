// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Loading/GLevelLoadingOpenLevel.h"

#include "Level/GLevelSubsystem.h"

FGLevelLoadingOpenLevel::FGLevelLoadingOpenLevel()
	: FGLevelLoadingBase(0)
{
	ProgressStage.Add(0.5);//MapLoading
	ProgressStage.Add(0.7);//NpcCreating
	ProgressStage.Add(0.9);//PlayerCreating
	ProgressStage.Add(1);//Done
}


void FGLevelLoadingOpenLevel::OnTick(float DeltaTime)
{
	FGLevelLoadingBase::OnTick(DeltaTime);
	if(UGLevelSubsystem::Get()->GetOpenLevelId() != GetLoadingLevelId())
	{
		return;
	}

	switch (CurrentStage)
	{
	case EStage::MapLoading:
		break;
	case EStage::LevelInit:
		{
			if(UGLevelSubsystem::Get()->TryInitLevel())
			{
				SetNextStage();
			}
		}
		break;
	case EStage::PlayerCreating:
		{
			if(UGLevelSubsystem::Get()->TryPreparePlayer())
			{
				SetCurrentStage(EStage::Done);
			}
		}
		break;
	default:
		SetCurrentStage(EStage::Done);
	}
	
}

void FGLevelLoadingOpenLevel::OnMapLoaded()
{
	if(CurrentStage == EStage::MapLoading)
	{
		SetNextStage();
	}
}
