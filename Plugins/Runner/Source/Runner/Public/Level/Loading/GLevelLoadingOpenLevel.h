// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GLevelLoadingBase.h"

class FGLevelLoadingOpenLevel : public FGLevelLoadingBase
{
	friend class UGLevelSubsystem;
public:
	FGLevelLoadingOpenLevel();
	virtual void OnTick(float DeltaTime) override;
protected:
	virtual void OnMapLoaded() override;

public:
	enum EStage
	{
		MapLoading,				//加载场景
		LevelInit,				//动态创建，创建Npc或者其它之类
		PlayerCreating,			//玩家是否准备好
		Done,
		Num
	};
private:
	//void OnPostLoadMapWithWorld(UWorld* World);
private:
	bool IsLoaded = false;
};