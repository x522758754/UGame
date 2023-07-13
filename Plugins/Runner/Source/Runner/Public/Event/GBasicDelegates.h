#pragma once

#include "CoreMinimal.h"


class AGPlayerControllerBasic;

class RUNNER_API FGBasicDelegates
{
public:	//Level加载事件
	DECLARE_DELEGATE_OneParam(FLevelLoadingEvent, int /*LevelId*/);

	static TFunction<void()> LevelLoadingLoadMapComplete;
	static TFunction<bool()> LevelLoadingSceneReady;		//场景是否准备好
	static TFunction<bool()> LevelLoadingSceneActorReady;	//场景是否准备好
	static TFunction<bool()> IsPlayerCharacterSpawned;		//玩家的角色是否创建
	static TFunction<bool()> NoticeLoadEndToGameSever;		//通过GameSever客户端场景加载完成
	static TFunction<void(const FString&)> OnGetMiGuToken;	//在获取咪咕token
	static TFunction<void(const FString&)> OnGetMiGuCommonInfo;//在获取咪咕通用信息
	static TFunction<void(const FString&)> OnGetMiGuLoginInfo; //获取咪咕用户登录信息
	static TFunction<void()> Connect2CenterServer;			//连接中心服务器
	static TFunction<void(int32 LevelId)> OnLoadLevelEnd;	//加载场景结束
	
	
public:
	static TFunction<FTransform(AGPlayerControllerBasic*)> FunctionGetBornTransform;
};