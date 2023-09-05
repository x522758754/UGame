// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Sound/GSoundDef.h"
#include "GClientSaveGame.generated.h"



/**
 * 一些全局的逻辑参数和数据，client data center
 */
UCLASS(BlueprintType, Blueprintable)
class UGClientSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGClientSaveGame();
	
	static FString SaveSlotName;
	
	UPROPERTY(EditAnywhere)
	FGSoundData SettingSoundData;
};