// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "Framework/Application/IInputProcessor.h"
#include "GInputSubsystem.generated.h"

class FGInputProcessor : public IInputProcessor
{
	friend class UGInputSubsystem;
public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override{}
	
	/** Mouse button press */
	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	/** Mouse button release */
	virtual bool HandleMouseButtonUpEvent( FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	/** Key down input */
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	
	/** Key up input */
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
private:
	bool AbsoluteToLocal(FVector2D& Local, const FVector2D& Absolute);
private:
	int MouseButtonDownEventObserverCount = 0;
	int MouseButtonUpEventObserverCount = 1;

	UGInputSubsystem* Owner = nullptr;
};
/**
 * 
 */
UCLASS()
class RUNNER_API UGInputSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FGInputProcessor;
public:
	UFUNCTION(BlueprintPure, Category = UGInputSubsystem)
	static UGInputSubsystem* Get();
protected:
	static TWeakObjectPtr<UGInputSubsystem> s_Instance;
	

	virtual void OnGameInstanceInit() override;
	virtual void Deinitialize() override;
protected:
	void RegisterInputPreProcessor();
public:
	UFUNCTION()
	void ObserveMouseButtonDown(const FString& Name);

	UFUNCTION()
	void ObserveMouseButtonUp(const FString& Name);

	UFUNCTION()
	void UnObserveMouseButtonDown(const FString& Name);

	UFUNCTION()
	void UnObserveMouseButtonUp(const FString& Name);

private:
	TSharedPtr<FGInputProcessor> InputProcessor;
};

