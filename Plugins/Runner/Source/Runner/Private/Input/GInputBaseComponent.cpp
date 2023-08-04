// Fill out your copyright notice in the Description page of Project Settings.

#include "Input//GInputBaseComponent.h"

#include "System/GCommonFunctions.h"
#include "Config/GGameConfigSettings.h"
#include "Player/GPlayerController.h"


// Sets default values for this component's properties
UGInputBaseComponent::UGInputBaseComponent()
{

}

// Called when the game starts
void UGInputBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}


bool UGInputBaseComponent::OnTick(float DeltaTime)
{
	return false;
}


bool UGInputBaseComponent::OnInputKey_Implementation(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	return false;
}

/** Handles a touch screen action */
bool UGInputBaseComponent::OnInputTouch_Implementation(ETouchIndex::Type TouchIndex, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, int32 TouchpadIndex)
{
	return false;
}

/** Handles a controller axis input */
bool UGInputBaseComponent::OnInputAxis_Implementation(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return false;
}

/** Handles motion control */
bool UGInputBaseComponent::OnInputMotion_Implementation(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration)
{
	return false;
}

void UGInputBaseComponent::OnSlideScreen_Implementation(const FVector& Delta)
{

}

void UGInputBaseComponent::OnClick_Implementation(const FVector2D& Location)
{

}

void UGInputBaseComponent::OnPinch_Implementation(float PrevDistance, float CurrentDistance)
{
	
}

/** Handles a key press */
bool UGInputBaseComponent::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;
	}
	switch (EventType)
	{
	case IE_Pressed:
		PC->GetMousePosition(MousePressedLocation.X, MousePressedLocation.Y);
		break;
	case IE_Released:
	{
		FVector2D Location;
		if (PC->GetMousePosition(Location.X, Location.Y))
		{
			if (FVector2D::Distance(Location, MousePressedLocation) < UGGameConfigSettings::Get()->InputClickMaxDistance)
			{
				OnClick(Location);
			}
		}
		MousePressedLocation = FVector2D::ZeroVector;
	}
		break;
	default:
		break;
	}
	return OnInputKey(Key, EventType, AmountDepressed, bGamepad);
}

/** Handles a touch screen action */
bool UGInputBaseComponent::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	const ETouchIndex::Type TouchIndex = (ETouchIndex::Type)Handle;
	switch (Type)
	{
	case ETouchType::Type::Began:
		{
			m_TouchBeginLocationMap.Add(TouchIndex, TouchLocation);
			m_TouchLocationMap.Add(TouchIndex, TouchLocation);
			RefreshPinch(TouchIndex);
		}
		break;
	case ETouchType::Type::Moved:
		{
			if(m_TouchLocationMap.Num() == 1)
			{
				FVector2D* Value = m_TouchLocationMap.Find(TouchIndex);
				if (Value)
				{
					const FVector2D Delta = TouchLocation - *Value;
					OnSlideScreen(FVector(Delta.X, Delta.Y, 0));
					*Value = TouchLocation;
				}
			}
			else if(m_TouchLocationMap.Num() == 2)
			{
				FVector2D* Value = m_TouchLocationMap.Find(TouchIndex);
				if (Value)
				{
					*Value = TouchLocation;
					float PrevDistance = m_PinchDistance;
					RefreshPinch(TouchIndex);
					OnPinch(PrevDistance, m_PinchDistance);
				}
			}
		}
		break;
	case ETouchType::Type::Ended:
		{
			if (m_TouchBeginLocationMap.Num() == 1)
			{
				FVector2D* Value = m_TouchBeginLocationMap.Find(TouchIndex);
				if (Value)
				{
					if (FVector2D::Distance(*Value, TouchLocation) < UGGameConfigSettings::Get()->InputClickMaxDistance)
					{
						OnClick(TouchLocation);
					}
				}
			}
			m_TouchBeginLocationMap.Remove(TouchIndex);
			m_TouchLocationMap.Remove(TouchIndex);
			RefreshPinch(TouchIndex);
		}
		break;
	default:
		{
		}
		break;
	}
	
	return true;
}

void UGInputBaseComponent::RefreshPinch(ETouchIndex::Type TouchIndex)
{
	if (m_TouchLocationMap.Num() != 2 || m_TouchLocationMap.Find(TouchIndex) == nullptr)
	{
		return;
	}
	if (m_TouchLocationMap.Num() == 2)
	{
		auto it = m_TouchLocationMap.begin();
		FVector2D Location1 = it->Value;
		++it;
		FVector2D Location2 = it->Value;

		m_PinchDistance = FVector2D::Distance(Location1, Location2);
	}
}

/** Handles a controller axis input */
bool UGInputBaseComponent::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (Key == EKeys::MouseX)
	{
		OnSlideScreen(FVector(Delta, 0, 0));
	}
	else if (Key == EKeys::MouseY)
	{
		OnSlideScreen(FVector(0, -Delta, 0));
	}
	
	return OnInputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
}

void UGInputBaseComponent::GetTwoTouchDistance(float& distance, bool& isTwoTouch)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return;
	}
	bool touch1_isPre, touch2_isPre;
	FVector2D Location1, Location2;
	PC->GetInputTouchState(ETouchIndex::Touch1, Location1.X, Location1.Y, touch1_isPre);
	PC->GetInputTouchState(ETouchIndex::Touch2, Location2.X, Location2.Y, touch2_isPre);

	isTwoTouch = touch1_isPre && touch2_isPre;
	if (isTwoTouch)
	{
		distance = FVector2D::Distance(Location1, Location2);
	}
	else
	{
		distance = 0;
	}
}

bool UGInputBaseComponent::IsPressed()
{
	return !MousePressedLocation.IsZero() || m_TouchLocationMap.Num();
}

bool UGInputBaseComponent::GetTwoTouchCenter(FVector2D& CenterPos)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;
	}
	bool touch1_isPre, touch2_isPre;
	FVector2D Location1, Location2;
	PC->GetInputTouchState(ETouchIndex::Touch1, Location1.X, Location1.Y, touch1_isPre);
	PC->GetInputTouchState(ETouchIndex::Touch2, Location2.X, Location2.Y, touch2_isPre);

	bool isTwoTouch = touch1_isPre && touch2_isPre;

	if(isTwoTouch)
	{
		CenterPos = (Location1 + Location2) / 2.f;
	}
	
	return isTwoTouch;
}

/** Handles motion control */
bool UGInputBaseComponent::InputMotion(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration)
{
	return OnInputMotion(Tilt, RotationRate, Gravity, Acceleration);
}