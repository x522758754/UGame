// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GEventDef.generated.h"

UENUM()
enum class EGEventType
{
	Begin = 0,
	PawnInfoAdded = 1,				//param0: UGPawnInfoComponent*	
	PawnInfoRemoved = 2,			//param0: UGPawnInfoComponent*
	SceneLogicIsReady = 3,
	LevelLoadStart = 4,				//场景开始加载 param0: int.LevelId
	LevelLoadEnd = 5,				//场景加载完成
	LevelLoadComplete = 6,			//场景切换完成 param0: int.LevelId
	LevelUnLoad = 7,				//场景释放
	MapLoadComplete = 8,			//场景加载完成 param0: int.SceneId, param1: float.LoadTime, param2:Const FString&.MapName
	PlayerInfoAdded = 9,
	PlayerInfoRemoved = 10,

	End,
};