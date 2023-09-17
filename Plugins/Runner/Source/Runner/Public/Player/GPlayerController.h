// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GPlayerController.generated.h"

class UGInputBaseComponent;
class UGPathFollowingComponent;
/**
 * 
 */
UCLASS()
class RUNNER_API AGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void Tick(float DeltaTime) override;

public:
	void HandleKeyDownEvent(const FKeyEvent& InKeyEvent);
	void HandleKeyUpEvent(const FKeyEvent& InKeyEvent);

	void ShowVirtualJoystick(bool bVisible, UTexture2D* tex);
	virtual TSharedPtr<class SVirtualJoystick> CreateVirtualJoystick() override;
	virtual FVector2D GetVirtualJoyOffset() ;

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	UGInputBaseComponent* AddInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass, FName ComponentName);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void EnableInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass, bool Value);
	UFUNCTION(BlueprintCallable, Category = "Input")
	UGInputBaseComponent* GetInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void RemoveInputChildComponent(TSubclassOf<UGInputBaseComponent> ComponentClass);
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsInputChildComponentEnable(TSubclassOf<UGInputBaseComponent> ComponentClass);

protected:
	virtual void InitInputChildComponent();
	/** Handles a key press */
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	/** Handles a touch screen action */
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;
	/** Handles motion control */
	virtual bool InputMotion(const FVector& Tilt, const FVector& RotationRate, const FVector& Gravity, const FVector& Acceleration) override;


protected:
	UPROPERTY(VisibleAnywhere, Transient)
	TArray<UGInputBaseComponent*> InputChildren;
	
	TMap<ETouchIndex::Type, FVector2D> TouchLocationMap;
	
	TSharedPtr<class SVirtualJoystick_Ex> VirtualJoystick_Ex;

	/** Component used for moving along a path. */
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	UGPathFollowingComponent* PathFollowingComponent;
};
