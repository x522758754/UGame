// Copyright Epic Games, Inc. All Rights Reserved.

#include "LGGameMode.h"
#include "LGPlayerController.h"
#include "LGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALGGameMode::ALGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}