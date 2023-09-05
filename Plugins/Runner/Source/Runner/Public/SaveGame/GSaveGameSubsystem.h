// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "Sound/GSoundDef.h"
#include "GSaveGameSubsystem.generated.h"

class USaveGame;
class UGClientSaveGame;
/**
 * 
 */
UCLASS()
class UGSaveGameSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UGSaveGameSubsystem* Get();
protected:
	static TWeakObjectPtr<UGSaveGameSubsystem> s_Instance;

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override {return  true;}
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable, Category = SaveGameSubsystem)
	UGClientSaveGame* GetClientSaveGame();
	UFUNCTION(BlueprintCallable, Category = SaveGameSubsystem)
	void SaveClientSaveGame(UGClientSaveGame* data);
	UFUNCTION(BlueprintCallable, Category = SaveGameSubsystem)
	void SaveSettingData(const FGSoundData& data);

public:
	static void SaveGameToSlot(USaveGame* SaveGame, const FString& Name);
	USaveGame* GetSaveGameObject(const FString& Name, TSubclassOf<USaveGame> SaveGameClass);
	template<class SaveGame>
	SaveGame* GetSaveGameObject(const FString& Name)
	{
		return Cast<SaveGame>(GetSaveGameObject(Name, SaveGame::StaticClass()));
	}
private:
	UGClientSaveGame* LoadSaveGameData();
	void SaveSaveGameData(UGClientSaveGame* SaveGameIns);

private:
	TWeakObjectPtr<UGClientSaveGame> ClientSaveGame;
	UPROPERTY()
	TMap<FString, USaveGame*> SaveGameMap;	//Key为FileName
};
