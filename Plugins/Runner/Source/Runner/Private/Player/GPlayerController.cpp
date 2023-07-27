// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Camera/GPlayerCameraManager.h"
#include "Player/GPlayerState.h"

AGPlayerController::AGPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGPlayerCameraManager::StaticClass();
	bAutoManageActiveCameraTarget = false;
}

void AGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}
}
