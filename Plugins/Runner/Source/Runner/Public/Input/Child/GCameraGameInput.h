// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/GInputBaseComponent.h"
#include "GCameraGameInput.generated.h"


/**
 * 
 */
UCLASS()
class UGCameraGameInput : public UGInputBaseComponent
{
	GENERATED_BODY()

protected:
	virtual void OnSlideScreen_Implementation(const FVector& Delta) override;
	virtual void OnPinch_Implementation(float PrevDistance, float CurrentDistance) override;

	virtual bool OnInputAxis_Implementation(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);
};
