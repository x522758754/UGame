// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GCommonFunctions.h"

#include "System/GGameInstance.h"
#include "Player/GPlayerController.h"
#include "Camera/GPlayerCameraManager.h"

#include "Character/Hero/GHeroCharacter.h"


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

AGHeroCharacter* UGCommonFunctions::GetHeroCharacter()
{
	if(GetPlayerController())
	{
		return Cast<AGHeroCharacter>(GetPlayerController()->GetPawn());
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

EGDirection UGCommonFunctions::GetDirection(FVector From, FVector To)
{
	FVector Direction = To - From;
	if(Direction.IsNearlyZero())
	{
		return EGDirection::None;
	}
	
	float XDiff = To.X - From.X;
	float YDiff = To.Y - From.Y;
	if(FMath::Abs(XDiff) > FMath::Abs(YDiff))
	{
		if(XDiff > 0)
		{
			return EGDirection::Right;
		}
		else if(XDiff < 0)
		{
			return EGDirection::Left;
		}
	}
	else
	{
		if(YDiff > 0)
		{
			return EGDirection::Down;
		}
		else if(YDiff < 0)
		{
			return EGDirection::UP;
		}
	}

	return EGDirection::None;
}

FVector UGCommonFunctions::GetDirectionVector(EGDirection Direction)
{
	switch (Direction)
	{
	case EGDirection::Right:
		return FVector(1.f, 0.f, 0.f);
	case EGDirection::Left:
		return FVector(-1.f, 0.f, 0.f);
	case EGDirection::Down:
		return FVector(0.f, 1.f, 0.f);
	case EGDirection::UP:
		return FVector(0.f, -1.f, 0.f);
	}
	return FVector::ZeroVector;
}
