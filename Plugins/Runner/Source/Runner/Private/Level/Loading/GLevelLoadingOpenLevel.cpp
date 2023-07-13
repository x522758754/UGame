// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Loading/GLevelLoadingOpenLevel.h"

#include "GLevelSubsystem.h"

FGLevelLoadingOpenLevel::FGLevelLoadingOpenLevel()
{
	ProgressStage.Empty();
	ProgressStage.Add(0.5);//Loading
	ProgressStage.Add(0.7);//SceneActorReady
	ProgressStage.Add(0.8);//AllStreamingCompleted
	ProgressStage.Add(0.9);//SceneReady
	ProgressStage.Add(1);	//Done
}


void FGLevelLoadingOpenLevel::OnTick(float DeltaTime)
{
	FGLevelLoadingBase::OnTick(DeltaTime);
	if(UGLevelSubsystem::Get()->GetOpenLevelName() == GetLoadingLevelId())
	{
		return;
	}

	switch (CurrentStage)
	{
	case EStage::Loading:
		{
			SetCurrentStage(EStage::SceneActorReady);
		}
		break;
	case EStage::SceneActorReady:
		{
			SetCurrentStage(EStage::AllStreamingCompleted);
		}
		break;
	case EStage::AllStreamingCompleted:
		{
			SetCurrentStage(EStage::PlayerReady);
		}
		break;
	case EStage::PlayerReady:
		{
			SetCurrentStage(EStage::Done);
		}
		break;
	default:
		SetCurrentStage(EStage::Done);
	}
	
}
