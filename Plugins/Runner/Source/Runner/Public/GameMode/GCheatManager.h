// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "GCheatManager.generated.h"

/**
 * 
 */
UCLASS(Within = GPlayerController)
class UGCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	
	UFUNCTION(Exec)
	void D_ClientTravel(int Level, int GuidIndex=0, FString Ip="127.0.0.1", int Port=7788);

	UFUNCTION(Exec)
	void D_ChangeLevel(int LevelId);

	UFUNCTION(Exec)
	void D_CameraMovement();

	UFUNCTION(Exec)
	void D_Possess(int64 Guid);
};
