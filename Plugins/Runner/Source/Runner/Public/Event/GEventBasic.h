#pragma once

#include "CoreMinimal.h"
#include "GEventSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GEventBasic.generated.h"

UENUM()
enum class EGEventBasic	//GameBasic和GameSubsystem模块中用到的协议放在这里
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

	End,
};

UCLASS()
class RUNNER_API UGEventBasicFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	template <typename ...Args>
	static void Dispatch(EGEventBasic Event, Args... args){
		if(UGEventSubsystem* Subsystem = UGEventSubsystem::Get())
		{
			Subsystem->DispatchEvent(static_cast<int>(Event), std::forward<Args>(args)...);
		}
	}
};