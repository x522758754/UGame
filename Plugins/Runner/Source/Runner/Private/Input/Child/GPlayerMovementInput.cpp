// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Child/GPlayerMovementInput.h"

#include "GPlayerCameraManager.h"
#include "System/GCommonFunctions.h"
#include "Event/GEventBasic.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Player/GPlayerController.h"

UGPlayerMovementInput::UGPlayerMovementInput()
{
	m_fGamePadInputAxis_X = 0.0f;
	m_fGamePadInputAxis_Y = 0.0f;
}

bool UGPlayerMovementInput::OnTick(float DeltaTime)
{
	TickMovement(DeltaTime);
	return false;
}

float UGPlayerMovementInput::GetJoyInputAngle()
{
	return atan2(m_fGamePadInputAxis_Y, m_fGamePadInputAxis_X) * 180 / PI;
}

bool UGPlayerMovementInput::ShowJoyDirection()
{
	return FMath::Abs(m_fGamePadInputAxis_X) < 0.01f && FMath::Abs(m_fGamePadInputAxis_Y) < 0.01f;
}

void UGPlayerMovementInput::IgnoreJump(bool Ignore)
{
	IgnorePlayerJump = Ignore;
}

void UGPlayerMovementInput::TickMovement(float DeltaTime)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC ||PC->IsMoveInputIgnored())
	{
		return;
	}
	FVector2D joyOffset = PC->GetVirtualJoyOffset();
	if (MoveDirection || FMath::Abs(joyOffset.X) > 0.01f || FMath::Abs(joyOffset.Y) > 0.01f)
	{
		ACharacter* Character = PC->GetPawn<ACharacter>();
		if (!Character)
		{
			return;
		}
		if(Character->GetMovementComponent()->IsFalling())
		{
			return;
		}
		AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager();
		if (!CameraManager)
		{
			return;
		}
		// FRotator Rotation = CameraManager->GetCameraRotation();
		//
		// if(!Character || Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
		// {
		// 	Rotation = FRotator(0, Rotation.Yaw, 0);
		// }
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
		
		// FVector JoyMoceDir = m_fGamePadInputAxis_X * Right + m_fGamePadInputAxis_Y * Foward;
		FVector JoyMoveDir = joyOffset.X * Right + joyOffset.Y * Foward;
		JoyMoveDir.Normalize();
		Direction += JoyMoveDir;
		Character->AddMovementInput(Direction, 1);

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
	if (MoveDirection == 0 && (FMath::Abs(m_fGamePadInputAxis_X) > 0.01f || FMath::Abs(m_fGamePadInputAxis_Y) > 0.01f) && Key != EKeys::SpaceBar)
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
 			if(IgnorePlayerJump)
			{
				return false;
			}
			if (ACharacter* Character = PC->GetPawn<ACharacter>())
			{
				//Character->Jump();
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
			ACharacter* Character = PC->GetPawn<ACharacter>();
			if (Character)
			{
				Character->StopJumping();
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
	// if (MoveDirectin != 0 && (FMath::Abs(m_fGamePadInputAxis_X) < 0.01f || FMath::Abs(m_fGamePadInputAxis_Y) < 0.01f))
	// {
	// 	return false;
	// }
	// if (Key.IsGamepadKey())
	// {
	// 	if (EKeys::Gamepad_LeftX == Key)
	// 	{
	// 		m_fGamePadInputAxis_X = Delta;
	// 		
	// 	}else if (EKeys::Gamepad_LeftY == Key)
	// 	{
	// 		m_fGamePadInputAxis_Y = Delta;
	// 	}
	// }
	// return true;
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
