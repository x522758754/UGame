// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInputBaseComponent.generated.h"

class AGGameCameraActor;
UENUM()
enum class EInputPriority : uint8
{
	EI_Normal = 0,
	EI_Avatar = 10,
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGInputBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AGPlayerController;
public:	
	// Sets default values for this component's properties
	UGInputBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//BlueprintNativeEvent
	///** Override FunctionName_Implementation for C++ */
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput")
	void OnSlideScreen(const FVector& Delta); 
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput") 
	void OnPinch(float PrevDistance, float CurrentDistance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput")
	void OnClick(const FVector2D& Location);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput") /** Handles a key press */
	bool OnInputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput") /** Handles a touch screen action */
	bool OnInputTouch(ETouchIndex::Type Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, int32 TouchpadIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput") /** Handles a controller axis input */
	bool OnInputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OnInput") /** Handles motion control */
	bool OnInputMotion(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration);

	virtual void OnInputKeyDeactivate(){}
	virtual void SetEnable(bool Enable){IsEnable = Enable;}
	
protected:
	virtual bool OnTick(float DeltaTime);

protected:
	void GetTwoTouchDistance(float& distance, bool& isTwoTouch) ;
	bool IsPressed();
	bool GetTwoTouchCenter(FVector2D &CenterPos);
private:
	/** Handles a key press */
	bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad);

	/** Handles a touch screen action */
	bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex);

	/** Handles a controller axis input */
	bool InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);

	/** Handles motion control */
	bool InputMotion(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool IsEnable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	EInputPriority Priority = EInputPriority::EI_Normal;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coefficient")
	float GesturePinchCoefficient = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coefficient")
	float HeightOffsetCoefficient = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coefficient")
	float YawCoefficient = 0.15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coefficient")
	float PitchCoefficient = 0.15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coefficient")
	float MouseWheelAxisCoefficient = 5;

private:
	void RefreshPinch(ETouchIndex::Type TouchIndex);

	float m_PinchDistance;
	TMap<ETouchIndex::Type, FVector2D> m_TouchLocationMap;
	TMap<ETouchIndex::Type, FVector2D> m_TouchBeginLocationMap;

	FVector2D MousePressedLocation;

	bool InputKeyActivate = false;
};
