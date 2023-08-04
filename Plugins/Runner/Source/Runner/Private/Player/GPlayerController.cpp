// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "Config/GGameConfigSettings.h"
#include "System/GCommonFunctions.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Camera/GPlayerCameraManager.h"
#include "Player/GPlayerState.h"
#include "GameFramework/PlayerInput.h"
#include "Input/GInputBaseComponent.h"
#include "UI/SVirtualJoystick_Ex.h"
#include "GameFramework/InputSettings.h"
#include "Engine/Classes/GameFramework/TouchInterface.h"
#include "Input/Child/GPlayerMovementInput.h"

AGPlayerController::AGPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGPlayerCameraManager::StaticClass();
	bAutoManageActiveCameraTarget = false;
}

void AGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitInputChildComponent();
}

void AGPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager())
	{
		CameraManager->SetFollow(InPawn);
		CameraManager->SetConfig(UGGameConfigSettings::Get()->DefaultCameraId);
		CameraManager->SetCameraParam(UGGameConfigSettings::Get()->DefaultCameraParam, true);
	}
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

void AGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EInputPriority Priority = EInputPriority::EI_Normal;
	for(const auto Child : InputChildren)
	{
		if (Child->Priority < Priority)
		{
			break;
		}
		if(Child->IsEnable)
		{
			if (Child->OnTick(DeltaTime))
			{
				Priority = Child->Priority;
			}
		}
	}
}

void AGPlayerController::HandleKeyDownEvent(const FKeyEvent& InKeyEvent)
{
	//FInputKeyParams Params;
	// Params.Key = InKeyEvent.GetKey();
	// if(InKeyEvent.IsRepeat())
	// {
	// 	Params.Event = EInputEvent::IE_Repeat;
	// }
	// else
	// {
	// 	Params.Event = EInputEvent::IE_Pressed;
	// }
	// EInputEvent EventType = Params.Event;
	// float AmountDepressed = Params.Delta.X;
	// bool bGamepad = Params.bIsGamepadOverride;
	// InputKey(Params);

	InputKey(InKeyEvent.GetKey(), InKeyEvent.IsRepeat() ? EInputEvent::IE_Repeat : EInputEvent::IE_Pressed, 0, false);
}

void AGPlayerController::HandleKeyUpEvent(const FKeyEvent& InKeyEvent)
{
	// FInputKeyParams Params;
	// Params.Key = InKeyEvent.GetKey();
	// Params.Event = EInputEvent::IE_Released;
	// EInputEvent EventType = Params.Event;
	// float AmountDepressed = Params.Delta.X;
	// bool bGamepad = Params.bIsGamepadOverride;
	// InputKey(Params);
	InputKey(InKeyEvent.GetKey(), EInputEvent::IE_Released, 0, false);
}


TSharedPtr<class SVirtualJoystick> AGPlayerController::CreateVirtualJoystick()
{

	return SNew(SVirtualJoystick_Ex);
}

FVector2D AGPlayerController::GetVirtualJoyOffset()
{
	if (VirtualJoystick_Ex.IsValid())
	{
		return VirtualJoystick_Ex->GetThumbOffsetPerCent();
	}
	return  FVector2D::ZeroVector;
}

void AGPlayerController::InitInputChildComponent()
{
	AddInputChildComponent(UGPlayerMovementInput::StaticClass(), TEXT("GPlayerMovementInput"));
}

void AGPlayerController::ShowVirtualJoystick(bool bVisible, UTexture2D* Tex)
{
	if (bVisible && (!VirtualJoystick.IsValid() || !VirtualJoystick->GetParentWidget()) && Tex)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			if (CurrentTouchInterface == nullptr)
			{
				FSoftObjectPath DefaultTouchInterfaceName = GetDefault<UInputSettings>()->DefaultTouchInterface;
				if (DefaultTouchInterfaceName.IsValid())
				{
					CurrentTouchInterface = LoadObject<UTouchInterface>(NULL, *DefaultTouchInterfaceName.ToString());
				}
			}
			if (CurrentTouchInterface)
			{
				if(!VirtualJoystick.IsValid())
				{
					VirtualJoystick = CreateVirtualJoystick();
				}
				LocalPlayer->ViewportClient->AddViewportWidgetContent(VirtualJoystick.ToSharedRef());
				ActivateTouchInterface(CurrentTouchInterface);
			}
			VirtualJoystick_Ex = StaticCastSharedPtr<SVirtualJoystick_Ex>(VirtualJoystick);
			VirtualJoystick_Ex->SetDirectionTex(Tex);
		}
	}
	SetVirtualJoystickVisibility(bVisible);
}

bool AGPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	bool bResult = Super::InputKey(Key, EventType, AmountDepressed, bGamepad);

	EInputPriority Priority = EInputPriority::EI_Normal;

	if(EventType != EInputEvent::IE_Repeat)
	{
		UE_LOG(LogTemp, Display, TEXT("AGPlayerController::InputKey Key(%s),Event(%d)"), *Key.GetFName().ToString(), EventType);
	}
	for(const auto Input : InputChildren)
	{
		if (Input->Priority < Priority)
		{
			if(Input->InputKeyActivate)
			{
				Input->OnInputKeyDeactivate();
				Input->InputKeyActivate = false;
			}
			continue;
		}
		if(Input->IsEnable)
		{
			Input->InputKeyActivate = true;
			if (Key == EKeys::MouseWheelAxis || Key == EKeys::Gamepad_LeftX ||  Key == EKeys::Gamepad_LeftY)
			{
				//适配引擎修改：UGameViewportClient::InputAxis，APlayerController::InputAxis将废弃
				// if (Input->InputAxis(Key, Params.Delta.X, Params.DeltaTime, Params.NumSamples, bGamepad))
				// {
				// 	Priority = Input->Priority;
				// }
			}
			else
			{
				if (Input->InputKey(Key, EventType, AmountDepressed, bGamepad))
				{
					Priority = Input->Priority;
				}
			}
		}
	}

	return bResult;
}

bool AGPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	bool bResult = Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
	

	const ETouchIndex::Type TouchIndex = static_cast<ETouchIndex::Type>(Handle);
	switch (Type)
	{
	case ETouchType::Type::Began:
		TouchLocationMap.Add(TouchIndex, TouchLocation);
		break;
	case ETouchType::Type::Ended:
		TouchLocationMap.Remove(TouchIndex);
		break;
	default:
		break;
	}

	EInputPriority Priority = EInputPriority::EI_Normal;
	for(const auto Input : InputChildren)
	{
		if (Input->Priority < Priority)
		{
			break;
		}
		if(Input->IsEnable)
		{
			if(Input->InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex))
			{
				Priority = Input->Priority;
			}
		}
	}
	return bResult;
}

bool AGPlayerController::InputMotion(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration)
{
	bool bResult = Super::InputMotion(Tilt, RotationRate, Gravity, Acceleration);
	for(const auto Input : InputChildren)
	{
		if(Input->IsEnable)
		{
			if(Input->InputMotion(Tilt, RotationRate, Gravity, Acceleration))
			{
				break;
			}
		}
	}
	return bResult;
}


UGInputBaseComponent* AGPlayerController::AddInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass, FName ComponentName)
{
	for (auto child : InputChildren)
	{
		if(child->GetClass() == ComponentClass)
		{
			return child;
		}
	}
	
	UGInputBaseComponent* Component = nullptr;
	if(FUObjectThreadContext::Get().TopInitializer())
	{
		Component = Cast<UGInputBaseComponent>(
			CreateDefaultSubobject(ComponentName, ComponentClass, ComponentClass, true, false) );
	}
	else
	{
		Component = NewObject<UGInputBaseComponent>(this, ComponentClass, ComponentName);
		Component->RegisterComponent();
	}
	
	int InsertIndex = -1;
	for(int Index = 0;Index < InputChildren.Num(); Index++)
	{
		if(InputChildren[Index]->Priority < Component->Priority)
		{
			InsertIndex = Index;
			break;
		}
	}
	if(InsertIndex < 0)
	{
		InputChildren.Add(Cast<UGInputBaseComponent>(Component));
	}
	else
	{
		InputChildren.Insert(Component, InsertIndex);
	}
	return Component;
}

UGInputBaseComponent* AGPlayerController::GetInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass)
{
	if (!ComponentClass)
	{
		return nullptr;
	}
	for (auto child : InputChildren)
	{
		if(child->GetClass() == ComponentClass)
		{
			return child;
		}
	}
	return nullptr;
}

void AGPlayerController::EnableInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass, bool Value)
{
	if (!ComponentClass)
	{
		return;
	}
	for (auto child : InputChildren)
	{
		if(child->GetClass() == ComponentClass)
		{
			child->SetEnable(Value);
			return;
		}
	}
	if (Value)
	{
		AddInputChildComponent(ComponentClass, ComponentClass->GetFName());
	}
}

void AGPlayerController::RemoveInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass)
{
	for (auto child : InputChildren)
	{
		if(child->GetClass() == ComponentClass)
		{
			InputChildren.Remove(child);
			return;
		}
	}
}

bool AGPlayerController::IsInputChildComponentEnable(TSubclassOf<UGInputBaseComponent> ComponentClass)
{
	for (auto child : InputChildren)
	{
		if(child->GetClass() == ComponentClass)
		{
			return child->IsEnable;
		}
	}
	return false;
}
