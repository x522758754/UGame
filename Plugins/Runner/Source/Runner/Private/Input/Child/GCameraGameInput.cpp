// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Child/GCameraGameInput.h"

#include "GPlayerCameraManager.h"
#include "System/GCommonFunctions.h"

void UGCameraGameInput::OnSlideScreen_Implementation(const FVector& Delta)
{
	if(AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager())
	{
		CameraManager->SetPitchDelta(-Delta.Y * PitchCoefficient, true);
		CameraManager->SetYawDelta(Delta.X * YawCoefficient, true);
	}
}

void UGCameraGameInput::OnPinch_Implementation(float PrevDistance, float CurrentDistance)
{
	if(AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager())
	{
		CameraManager->SetDistanceDelta((PrevDistance - CurrentDistance) * GesturePinchCoefficient);
	}
}

bool UGCameraGameInput::OnInputAxis_Implementation(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (EKeys::MouseWheelAxis == Key)
	{
		if(AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager())
		{
			CameraManager->SetDistanceDelta(-Delta * MouseWheelAxisCoefficient* MouseWheelAxisCoefficient);
		}
	}
	return false;
}