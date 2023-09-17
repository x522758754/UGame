// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Child/GPlayerMovementInput.h"

#include "Camera/GPlayerCameraManager.h"
#include "System/GCommonFunctions.h"
#include "Character/Hero/GHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Level/GLevelFunctions.h"
#include "Level/GLevelSubsystem.h"

#include "Navigate/GNavigateFunctions.h"

#include "Player/GPlayerController.h"

UGPlayerMovementInput::UGPlayerMovementInput()
{
}

bool UGPlayerMovementInput::OnTick(float DeltaTime)
{
	TickMovement(DeltaTime);
	return false;
}

void UGPlayerMovementInput::TickMovement(float DeltaTime)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC ||PC->IsMoveInputIgnored())
	{
		return;
	}
	AGHeroCharacter* Character = PC->GetPawn<AGHeroCharacter>();
	if (!Character)
	{
		return;
	}
	if(Character->GetMovementComponent()->IsFalling())
	{
		return;
	}
	FVector2D joyOffset = PC->GetVirtualJoyOffset();
	if (MoveDirection || FMath::Abs(joyOffset.X) > 0.01f || FMath::Abs(joyOffset.Y) > 0.01f)
	{
		AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager();
		if (!CameraManager)
		{
			return;
		}
		const FVector Foward = FVector(0, -1, 0);
		const FVector Right = FVector(1, 0, 0);
		FVector Direction = FVector::ZeroVector;
		if (MoveDirection & EDirection::ED_Forward)
		{
			Direction += Foward;
		}
		if (MoveDirection & EDirection::ED_Backward)
		{
			Direction -= Foward;
		}
		if (MoveDirection & EDirection::ED_Left)
		{
			Direction -= Right;
		}
		if (MoveDirection & EDirection::ED_Right)
		{
			Direction += Right;
		}
		
		FVector JoyMoveDir = joyOffset.X * Right + joyOffset.Y * Foward;
		JoyMoveDir.Normalize();
		Direction += JoyMoveDir;
		Character->Move(Direction);

		//UGEventBasicFunctions::Dispatch(EGEventType::PlayerMovement, Direction);
	}
}


bool UGPlayerMovementInput::OnInputKey_Implementation(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
 	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;	
	}
	switch (EventType)
	{
	case EInputEvent::IE_Pressed:
	{
 		if (Key == EKeys::W)
		{
			MoveDirection |= EDirection::ED_Forward;
		}
		else if (Key == EKeys::S)
		{
			MoveDirection |= EDirection::ED_Backward;
		}
		else if (Key == EKeys::A)
		{
			MoveDirection |= EDirection::ED_Left;
		}
		else if (Key == EKeys::D)
		{
			MoveDirection |= EDirection::ED_Right;
		}
		else if (Key == EKeys::SpaceBar)
		{
			if(!UGNavigateFunctions::IsNavigating())
			{
				UGLevelSubsystem::Get()->NavigateToGoal();
			}
		}
		if(MoveDirection > 0)
		{
			//UGEventBasicFunctions::Dispatch(EGEventType::PlayerMoveStart, MoveDirection);
		}	
	}
		break;
	case EInputEvent::IE_Released:
	{
		if (Key == EKeys::W)
		{
			MoveDirection &= ~EDirection::ED_Forward;
		}
		else if (Key == EKeys::S)
		{
			MoveDirection &= ~EDirection::ED_Backward;
		}
		else if (Key == EKeys::A)
		{
			MoveDirection &= ~EDirection::ED_Left;
		}
		else if (Key == EKeys::D)
		{
			MoveDirection &= ~EDirection::ED_Right;
		}
		else if (Key == EKeys::SpaceBar)
		{
			if(UGNavigateFunctions::IsNavigating())
			{
				UGNavigateFunctions::AbortMove();
			}
		}
	}
		break;
	default:
		break;
	}
	return false;
}


bool UGPlayerMovementInput::OnInputAxis_Implementation(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return false;
}

void UGPlayerMovementInput::OnInputKeyDeactivate()
{
	MoveDirection = 0;
}

void UGPlayerMovementInput::SetEnable(bool Enable)
{
	Super::SetEnable(Enable);
	if(!Enable)
	{
		MoveDirection = 0;
	}
}
