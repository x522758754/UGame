// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/GSaveGameSubsystem.h"
#include "SaveGame/GClientSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "System/GGameInstance.h"

TWeakObjectPtr<UGSaveGameSubsystem> UGSaveGameSubsystem::s_Instance;
UGSaveGameSubsystem* UGSaveGameSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGSaveGameSubsystem>();
	}
	return s_Instance.Get();
}

UGClientSaveGame* UGSaveGameSubsystem::LoadSaveGameData()
{
	UGClientSaveGame* SaveGameIns = Cast<UGClientSaveGame>(UGameplayStatics::LoadGameFromSlot(UGClientSaveGame::SaveSlotName, 0));
	if (!SaveGameIns)
	{
		SaveGameIns = Cast<UGClientSaveGame>(UGameplayStatics::CreateSaveGameObject(UGClientSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveGameIns, UGClientSaveGame::SaveSlotName, 0);
	}

	return SaveGameIns;
}

void UGSaveGameSubsystem::SaveSaveGameData(UGClientSaveGame* SaveGameIns)
{
	if (SaveGameIns == nullptr)
	{
		return;
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGameIns, UGClientSaveGame::SaveSlotName, 0);
}

UGClientSaveGame* UGSaveGameSubsystem::GetClientSaveGame()
{
	if (ClientSaveGame.IsValid())
	{
		return ClientSaveGame.Get();
	}
	else
	{
		//游戏中创建的UObject，在一定时间后会被UE自动GC，除非AddToRoot
		//对于非频繁使用且和其他逻辑关联少的UObject，使用弱指针判断有效性
		UGClientSaveGame* SaveObj = LoadSaveGameData();
		ClientSaveGame = MakeWeakObjectPtr(SaveObj);
		return SaveObj;
	}
}

void UGSaveGameSubsystem::SaveClientSaveGame(UGClientSaveGame* data)
{
	SaveSaveGameData(data);
}

void UGSaveGameSubsystem::SaveSettingData(const FGSoundData& data)
{
	if (UGClientSaveGame* savegame = GetClientSaveGame())
	{
		savegame->SettingSoundData = data;
		SaveSaveGameData(savegame);
	}
}

void UGSaveGameSubsystem::SaveGameToSlot(USaveGame* SaveGame, const FString& Name)
{
	if (SaveGame == nullptr)
	{
		return;
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGame, Name, 0);
}

void UGSaveGameSubsystem::Deinitialize()
{
	for(auto KeyValue : SaveGameMap)
	{
		if(IsValid(KeyValue.Value))
		{
			UGameplayStatics::SaveGameToSlot(KeyValue.Value, KeyValue.Key, 0);
		}
	}
}

USaveGame* UGSaveGameSubsystem::GetSaveGameObject(const FString& Name, TSubclassOf<USaveGame> SaveGameClass)
{
	if(const auto Value = SaveGameMap.Find(Name))
	{
		return *Value;
	}
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(Name, 0);
	if (!SaveGame)
	{
		SaveGame = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
		UGameplayStatics::SaveGameToSlot(SaveGame, Name, 0);
	}
	SaveGameMap.Add(Name, SaveGame);
	return SaveGame;
}

