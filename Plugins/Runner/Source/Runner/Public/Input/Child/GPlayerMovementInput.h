// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/GInputBaseComponent.h"
#include "GPlayerMovementInput.generated.h"

/**
 * 
 */
UCLASS()
class UGPlayerMovementInput : public UGInputBaseComponent
{
	GENERATED_BODY()
	
public:
	UGPlayerMovementInput();

public:
	enum EDirection : uint32
	{
		ED_Left = 0x0001,
		ED_Right = 0x0002,
		ED_Forward = 0x0004,
		ED_Backward = 0x0008,
		ED_Up = 0x00010,
		ED_Down = 0x00020,
	};

protected:
	virtual bool OnInputKey_Implementation(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	virtual bool OnInputAxis_Implementation(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;
	virtual bool OnTick(float DeltaTime) override;
	virtual void OnInputKeyDeactivate() override;
	virtual void SetEnable(bool Enable) override;
public:
	float GetJoyInputAngle();
	bool ShowJoyDirection();
	void IgnoreJump(bool Ignore);
private:
	void TickMovement(float DeltaTime);
private:
	int MoveDirection;
	float m_fGamePadInputAxis_X;
	float m_fGamePadInputAxis_Y;
	bool IgnorePlayerJump;
};
