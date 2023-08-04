// Fill out your copyright notice in the Description page of Project Settings.

#include "System/GGameInstance.h"

#include "Config/GGameConfigSettings.h"
#include "Level/GLevelSubsystem.h"
#include "Level/Data/GLevelData.h"
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

	//UGLevelSubsystem::Get()->ChangeLevel(UGGameConfigSettings::Get()->DefaultLevelId);
}

void UGGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);

	//GetWorld()->CreatePhysicsScene();
}

void UGGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
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
