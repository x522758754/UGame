// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelSubsystem.h"

#include "System/GCommonFunctions.h"
#include "System/GAssetManager.h"
#include "Config/GConfigData.h"
#include "Event/GEventDef.h"
#include "Config/GGameConfigSettings.h"
#include "Character/Hero/GHeroFunctions.h"
#include "Character/Hero/GHeroCharacter.h"
#include "Camera/GPlayerCameraManager.h"
#include "Player/GPlayerController.h"
#include "Level/Data/GLevelData.h"
#include "System/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Loading/GLevelLoadingOpenLevel.h"

TWeakObjectPtr<UGLevelSubsystem> UGLevelSubsystem::s_Instance =	nullptr;

bool UGLevelSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UGLevelSubsystem::OnGameInstanceInit()
{
	Super::OnGameInstanceInit();

	const TFunction<void(const float, const FString&)> ObserverMapLoadComplete = [this](const float LoadTime, const FString& MapName)
	{
		OnMapLoadComplete(LoadTime, MapName);
	};
	EventObserverHolder.Observe(EGEventType::MapLoadComplete, ObserverMapLoadComplete);

	const TFunction<void(const float, const FString&)> ObserverLevelLoadEnd = [this](const float LoadTime, const FString& MapName)
	{
		OnLevelLoaded();
	};
	EventObserverHolder.Observe(EGEventType::LevelLoadEnd, ObserverLevelLoadEnd);
}

void UGLevelSubsystem::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);
	if(CurrentLoading.IsValid())
	{
		CurrentLoading->OnTick(DeltaTime);
		if(CurrentLoading->GetProgress() >= 1)
		{
			CurrentLoading->LoadEnd();
			CurrentLoading = nullptr;
		}
	}
}

void UGLevelSubsystem::OnMapLoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogTemp, Display, TEXT("UGLevelSubsystem::OnMapLoadComplete LoadTime:%f MapName:%s"), LoadTime, *MapName);
}

void UGLevelSubsystem::OnLevelLoaded()
{
	UE_LOG(LogTemp, Display, TEXT("UGLevelSubsystem::OnLevelLoaded"));
	//创建玩家
	if(UGConfigData::Get()->LevelConfigs.Contains(OpenLevelId))
	{
		const FGLevelConfig& Cfg = UGConfigData::Get()->LevelConfigs[OpenLevelId];
		/*AGHeroCharacter *Hero = UGHeroFunctions::SpawnHero(UGGameConfigSettings::Get()->DefaultHeroId, Cfg.BornTransform);
		if(Hero)
		{
			if(AGPlayerController* PlayerController = UGCommonFunctions::GetPlayerController())
			{
				PlayerController->Possess(Hero);
				//PlayerController->SetPawn(Hero);
			}
			//设置相机
		}*/
	}
}

UGLevelSubsystem* UGLevelSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGLevelSubsystem>();
	}
	return s_Instance.Get();
}

int32 UGLevelSubsystem::GetOpenLevelId() const
{
	return OpenLevelId;
}

void UGLevelSubsystem::ChangeLevel(int32 LevelId)
{
	if(LevelId != OpenLevelId)
	{
		if(!UGConfigData::Get()->LevelConfigs.Contains(LevelId))
		{
			UE_LOG(LogTemp, Error, TEXT("UGLevelSubsystem::ChangeLevel LevelId:%d"), LevelId);
			return;
		}
		
		const FGLevelConfig& Cfg = UGConfigData::Get()->LevelConfigs[LevelId];
		BeginLoading(ELoadingType::OpenLevel, LevelId);
		OpenLevelId = LevelId;
		OpenLevelLoaded = false;
		UGameplayStatics::OpenLevel(GetWorld(), FName(Cfg.GetMapName()));
	}
}

void UGLevelSubsystem::BeginLoading(ELoadingType Type, int32 LevelId)
{
	CurrentLoadingType = Type;
	if (Type < ELoadingType::Dynamic || Type >= ELoadingType::Num)
	{
		UE_LOG(LogTemp, Fatal, TEXT("UGLevelSubsystem::SetLoadingType Error Type, %d"), Type);
		return;
	}
	switch (Type)
	{
	case ELoadingType::OpenLevel:
		CurrentLoading = MakeShared<FGLevelLoadingOpenLevel>();
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("UGLevelSubsystem::SetLoadingType %d"), Type);
		return;
	}
	CurrentLoading->Begin(LevelId);
}

