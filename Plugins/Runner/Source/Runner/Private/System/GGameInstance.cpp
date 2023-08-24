// Fill out your copyright notice in the Description page of Project Settings.

#include "System/GGameInstance.h"

#include "Config/GGameConfigSettings.h"
#include "Level/GLevelSubsystem.h"
#include "Level/Data/GLevelData.h"
#include "Level/GLevelFunctions.h"
#include "System/GAssetManager.h"
#include "System/GGameInstanceSubsystem.h"


TWeakObjectPtr<UGGameInstance> UGGameInstance::GameInstancePtr;

UGGameInstance* UGGameInstance::Get()
{
	return GameInstancePtr.Get();
}

UGGameInstance::UGGameInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UGGameInstance::Init()
{
	Super::Init();
	GameInstancePtr = this;
	
	const TArray<UGGameInstanceSubsystem*>& SubsystemArray = GetSubsystemArray<UGGameInstanceSubsystem>();
	for(UGGameInstanceSubsystem* Subsystem : SubsystemArray)
	{
		Subsystem->OnGameInstanceInit();
	}
}

void UGGameInstance::OnStart()
{
	Super::OnStart();

	UGLevelSubsystem::Get()->ChangeLevel(UGGameConfigSettings::Get()->DefaultLevelId);
}

void UGGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);

	FString FormatMapName = UWorld::RemovePIEPrefix(MapName);
	int32 LevelId = UGLevelFunctions::GetLevelId(FormatMapName);
	UGEventBasicFunctions::Dispatch(EGEventType::MapLoadComplete, LevelId, LoadTime, MapName);

	UE_LOG(LogTemp, Log, TEXT("UGGameInstanceBasic::LoadComplete LevelId=%d,LoadTime=%f,MapName=%s"), LevelId, LoadTime, *MapName);
}

void UGGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);

	if(OldWorld)
	{
		UE_LOG(LogTemp, Log, TEXT("UGGameInstance::OnWorldChanged OldWorld is %s, LevelId:%d"), *OldWorld->GetName(), UGLevelFunctions::GetLevelId(OldWorld->GetName()));
	}
	if(NewWorld)
	{
		UE_LOG(LogTemp, Log, TEXT("UGGameInstance::OnWorldChanged NewWorld is %s, LevelId:%d"), *NewWorld->GetName(), UGLevelFunctions::GetLevelId(NewWorld->GetName()));
	}
}

TStatId UGGameInstance::GetStatId() const
{
	return GetStatID();
}

void UGGameInstance::Tick(float DeltaTime)
{
	const TArray<UGGameInstanceSubsystem*>& SubsystemArray = GetSubsystemArray<UGGameInstanceSubsystem>();
	for(UGGameInstanceSubsystem* Subsystem : SubsystemArray)
	{
		Subsystem->OnTick(DeltaTime);
	}
}
