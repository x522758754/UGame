// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GLevelLoadingBase.h"

class FGLevelLoadingOpenLevel : public FGLevelLoadingBase
{
public:
	FGLevelLoadingOpenLevel();
	virtual void OnTick(float DeltaTime) override;

public:
	enum EStage
	{
		Loading,				//场景加载
		SceneActorReady,		//创建SceneActor
		AllStreamingCompleted,	//周围场景是否加载完毕
		PlayerReady,			//玩家是否准备好
		Done,
		Num
	};
private:
	//void OnPostLoadMapWithWorld(UWorld* World);
private:
	bool IsLoaded = false;
};