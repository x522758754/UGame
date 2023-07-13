// Fill out your copyright notice in the Description page of Project Settings.

#include "System/GGameInstance.h"

#include "Config/GGameConfigSettings.h"
#include "Level/GLevelSubsystem.h"
#include "Level/Data/GLevelData.h"
#include "System/GAssetManager.h"


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

	UGLevelData* Data = UGAssetManager::Get()->GetAsset(UGGameConfigSettings::Get()->InitLevelData);
	if(Data)
	{
		UGLevelSubsystem::Get()->ChangeLevel(Data->ConstructTravelURL());
	}
}

void UGGameInstance::OnStart()
{
	Super::OnStart();
}

void UGGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);
}

void UGGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
}
