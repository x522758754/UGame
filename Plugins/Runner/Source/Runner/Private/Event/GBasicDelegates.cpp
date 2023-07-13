#include "Event/GBasicDelegates.h"

TFunction<void()> FGBasicDelegates::LevelLoadingLoadMapComplete = nullptr;
TFunction<bool()> FGBasicDelegates::LevelLoadingSceneReady = nullptr;
TFunction<bool()> FGBasicDelegates::LevelLoadingSceneActorReady = nullptr;
TFunction<bool()> FGBasicDelegates::IsPlayerCharacterSpawned = nullptr;
TFunction<bool()> FGBasicDelegates::NoticeLoadEndToGameSever = nullptr;
TFunction<void(const FString&)> FGBasicDelegates::OnGetMiGuToken = nullptr;
TFunction<void(const FString&)> FGBasicDelegates::OnGetMiGuCommonInfo = nullptr;
TFunction<void(const FString&)> FGBasicDelegates::OnGetMiGuLoginInfo = nullptr;
TFunction<void()> FGBasicDelegates::Connect2CenterServer = nullptr;
TFunction<void(int32 LevelID)> FGBasicDelegates::OnLoadLevelEnd = nullptr;


TFunction<FTransform(AGPlayerControllerBasic*)> FGBasicDelegates::FunctionGetBornTransform = nullptr;