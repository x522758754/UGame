// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GCommonFunctions.h"

#include "System/GGameInstance.h"
#include "Player/GPlayerController.h"
#include "Camera/GPlayerCameraManager.h"


UWorld* UGCommonFunctions::GetCurWorld()
{
	if(UGGameInstance::Get())
	{
		return UGGameInstance::Get()->GetWorld();
	}
	return nullptr;
}

AGPlayerController* UGCommonFunctions::GetPlayerController()
{
	if(UGGameInstance::Get())
	{
		return Cast<AGPlayerController>(UGGameInstance::Get()->GetFirstLocalPlayerController());
	}
	return nullptr;
}

AGPlayerCameraManager* UGCommonFunctions::GetPlayerCameraManager()
{
	if(GetPlayerController())
	{
		return Cast<AGPlayerCameraManager>(GetPlayerController()->PlayerCameraManager);
	}

	return nullptr;
}
