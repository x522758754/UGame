// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMode/GCheatManager.h"

#include "System/GCommonFunctions.h"
#include "Character/Hero/GHeroSubsystem.h"
#include "Level/GLevelSubsystem.h"
#include "Player/GPlayerController.h"
#include "Character/Hero/GHeroCharacter.h"


void UGCheatManager::D_ClientTravel(int Level, int GuidIndex, FString Ip, int Port)
{
	int64 Guid = 0x0010 * GuidIndex + 0x0001;
	
}

void UGCheatManager::D_ChangeLevel(int LevelId)
{
	UGLevelSubsystem::Get()->ChangeLevel(LevelId);
}

void UGCheatManager::D_CameraMovement()
{

}

void UGCheatManager::D_Possess(int64 Guid)
{
	AGHeroCharacter* Hero = UGHeroSubsystem::Get()->GetHeroCharacter(Guid);
	if(Hero)
	{
		UGCommonFunctions::GetPlayerController()->Possess(Hero);
	}
}
