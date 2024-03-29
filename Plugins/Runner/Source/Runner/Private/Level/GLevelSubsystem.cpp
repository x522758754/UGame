// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelSubsystem.h"

#include "Actor/Router/GRouter.h"

#include "Character/Component/GPathFollowingComponent.h"

#include "Config/GGameConfigSettings.h"
#include "Character/Hero/GHeroFunctions.h"
#include "Character/Hero/GHeroCharacter.h"
#include "System/GCommonFunctions.h"
#include "System/GAssetManager.h"
#include "System/GGameInstance.h"
#include "Player/GPlayerController.h"
#include "Config/GConfigDataAsset.h"
#include "Event/GEventDef.h"
#include "Level/Data/GLevelData.h"
#include "Level/Data/GLevelNpcConfigAsset.h"
#include "Level/GLevelActor.h"
#include "Level/Loading/GLevelLoadingOpenLevel.h"
#include "Kismet/GameplayStatics.h"

#include "Navigate/GNavigateFunctions.h"


TWeakObjectPtr<UGLevelSubsystem> UGLevelSubsystem::s_Instance =	nullptr;

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
		if(!UGConfigDataAsset::Get()->LevelConfigs.Contains(LevelId))
		{
			UE_LOG(LogTemp, Error, TEXT("UGLevelSubsystem::ChangeLevel LevelId:%d"), LevelId);
			return;
		}
		
		const FGLevelConfig& Cfg = UGConfigDataAsset::Get()->LevelConfigs[LevelId];
		OnLevelBeginLoading(ELoadingType::OpenLevel, LevelId);
		OpenLevelId = LevelId;
		UGameplayStatics::OpenLevel(GetWorld(), FName(Cfg.GetMapName()));
	}
}

const FGLevelConfig* UGLevelSubsystem::GetCurrentLevelConfig() const
{
	return CurrentLevelConfig;
}

const UGLevelNpcConfigAsset* UGLevelSubsystem::GetCurrentLevelNpcConfigAsset() const
{
	return CurrentLevelNpcConfigAsset;
}

AActor* UGLevelSubsystem::GetRouterActor()
{
	if(!RouterActor.IsValid())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Name = TEXT("RouterActor");
		RouterActor = GetWorld()->SpawnActor<AActor>(SpawnParameters);
	}

	return RouterActor.Get();
}

bool UGLevelSubsystem::TryInitLevel()
{
	if(LevelActor.IsValid())
	{
		return true;
	}
	else
	{
		if(!CurrentLevelConfig || !CurrentLevelConfig->LevelActorClassPath.IsValid())
		{
			LevelActor = GetWorld()->SpawnActor<AGLevelActor>();	//创建默认的SceneActor
			UE_LOG(LogTemp, Error, TEXT("UGLevelSubsystem::TryInitLevel CurrentLevelConfig Error"));
		}
		else
		{
			if(TSubclassOf<AGLevelActor> LevelActorClass = UGAssetManager::LoadSubclass(CurrentLevelConfig->LevelActorClassPath))
			{
				LevelActor = GetWorld()->SpawnActor<AGLevelActor>(LevelActorClass);
			}
			else
			{
				LevelActor = GetWorld()->SpawnActor<AGLevelActor>();	//创建默认的SceneActor
				UE_LOG(LogTemp, Error, TEXT("UGLevelSubsystem::TryInitLevel Load LevelActorClass Error %s"), *CurrentLevelConfig->LevelActorClassPath.ToString());
			}
		}
	}
	return false;
}

bool UGLevelSubsystem::TryPreparePlayer()
{
	auto Pawn = UGCommonFunctions::GetPlayerController()->GetPawn();
	if(Pawn)
	{
		return true;
	}

	if(!CurrentLevelConfig)
	{
		return false;
	}

	AGHeroCharacter *Hero = UGHeroFunctions::SpawnHero(UGGameConfigSettings::Get()->DefaultHeroId, CurrentLevelConfig->BornTransform);
	if(Hero)
	{
		if(AGPlayerController* PlayerController = UGCommonFunctions::GetPlayerController())
		{
			PlayerController->Possess(Hero);
		}
		//设置相机
	}

	return true;
}

void UGLevelSubsystem::NavigateToGoal()
{
	if(CurrentLevelConfig->GoalLocation.IsNearlyZero())
	{
		return;
	}

	

	UGNavigateFunctions::NavigateToGoal(CurrentLevelConfig->GoalLocation, nullptr);
}

bool UGLevelSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UGLevelSubsystem::OnGameInstanceInit()
{
	Super::OnGameInstanceInit();

	const TFunction<void(int32, const float, const FString&)> ObserverMapLoadComplete = [this](int32 LevelId, const float LoadTime, const FString& MapName)
	{
		OnMapLoadComplete(LevelId, LoadTime, MapName);
	};
	EventObserverHolder.Observe(EGEventType::MapLoadComplete, ObserverMapLoadComplete);
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
			OnLevelLoaded();
		}
	}
}

void UGLevelSubsystem::OnLevelBeginLoading(ELoadingType Type, int32 LevelId)
{
	CurrentLoadingType = Type;
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
	CurrentLevelNpcConfigAsset = nullptr;
	CurrentLevelConfig = nullptr;

	UGEventBasicFunctions::Dispatch(EGEventType::LevelLoadStart, LevelId);
}

void UGLevelSubsystem::OnMapLoadComplete(int32 LevelId, const float LoadTime, const FString& MapName)
{
	if(CurrentLoading.IsValid())
	{
		if(CurrentLoading->GetLoadingLevelId() == LevelId)
		{
			CurrentLoading->OnMapLoaded();
		}
	}
	if(FGLevelConfig* LevelConfig = UGConfigDataAsset::Get()->LevelConfigs.Find(LevelId))
	{
		CurrentLevelNpcConfigAsset = UGAssetManager::LoadAsset(LevelConfig->NpcConfigAsset);
		CurrentLevelConfig = LevelConfig;
	}
}



void UGLevelSubsystem::OnLevelLoaded()
{
	UE_LOG(LogTemp, Display, TEXT("UGLevelSubsystem::OnLevelLoaded"));
	UGEventBasicFunctions::Dispatch(EGEventType::LevelLoadEnd, OpenLevelId);
}
