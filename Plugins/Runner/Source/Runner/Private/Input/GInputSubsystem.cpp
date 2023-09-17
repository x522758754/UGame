// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/GInputSubsystem.h"

#include "Player/GPlayerController.h"
#include "System/GCommonFunctions.h"
#include "System/GGameInstance.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Framework/Application/SlateUser.h"


bool FGInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if(MouseButtonDownEventObserverCount == 0)
	{
		return false;
	}
	FVector2D Position;
	if(AbsoluteToLocal(Position, MouseEvent.GetScreenSpacePosition()) == false)
	{
		return false;
	}
	//UGEventSubsystem::Get()->DispatchEvent(EGEventDefine::GlobalMouseButtonDown, Position);
	return false;
}

bool FGInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if(MouseButtonUpEventObserverCount == 0)
	{
		return false;
	}

	FVector2D Position;
	if(AbsoluteToLocal(Position, MouseEvent.GetScreenSpacePosition()) == false)
	{
		return false;
	}
	//UGEventSubsystem::Get()->DispatchEvent(EGEventDefine::GlobalMouseButtonUp, Position);

	//不吞掉输入 【点击按钮还是响应按钮事件,PlayerInput】
	return false;
}

bool FGInputProcessor::AbsoluteToLocal(FVector2D& Local, const FVector2D& Absolute)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;
	}
	
	FGeometry&& Geometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(PC);
	Local = Geometry.AbsoluteToLocal(Absolute);
	if(Local.X < 0 || Local.Y < 0)
	{
		return false;
	}

	const FVector2D& Size = Geometry.GetLocalSize();
	if(Local.X > Size.X || Local.Y > Size.Y)
	{
		return false;
	}
	return true;
}

bool FGInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;
	}
	TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(0);
	if(!SlateUser.IsValid())
	{
		return false;
	}
	if(!SlateUser->GetFocusedWidget().IsValid())
	{
		PC->HandleKeyDownEvent(InKeyEvent);
	}
	
	return false;
}

bool FGInputProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(!PC)
	{
		return false;
	}
	TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(0);
	if(!SlateUser.IsValid())
	{
		return false;
	}
	if(!SlateUser->GetFocusedWidget().IsValid())
	{
		PC->HandleKeyUpEvent(InKeyEvent);
	}	
	return false;
}

//----------------------------------------------------------------------------------------------------------------------

TWeakObjectPtr<UGInputSubsystem> UGInputSubsystem::s_Instance;
UGInputSubsystem* UGInputSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGInputSubsystem>();
	}
	return s_Instance.Get();
}

void UGInputSubsystem::OnGameInstanceInit()
{
	RegisterInputPreProcessor();
}

void UGInputSubsystem::Deinitialize()
{
	if (FSlateApplication::IsInitialized() && InputProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}
}

void UGInputSubsystem::RegisterInputPreProcessor()
{
	//优先级高于 Viewport
	InputProcessor = MakeShared<FGInputProcessor>();
	InputProcessor->Owner = this;
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor);
	}
}

void UGInputSubsystem::ObserveMouseButtonDown(const FString& Name)
{
	if(InputProcessor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("UGInputSubsystem::ListenMouseButtonDown %s"), *Name);
		InputProcessor->MouseButtonDownEventObserverCount += 1;
	}
}

void UGInputSubsystem::ObserveMouseButtonUp(const FString& Name)
{
	if(InputProcessor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("UGInputSubsystem::ListenMouseButtonUp %s"), *Name);
		InputProcessor->MouseButtonUpEventObserverCount += 1;
	}
}

void UGInputSubsystem::UnObserveMouseButtonDown(const FString& Name)
{
	if(InputProcessor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("UGInputSubsystem::UnObserveMouseButtonDown %s"), *Name);
		if(InputProcessor->MouseButtonDownEventObserverCount == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UGInputSubsystem::UnObserveMouseButtonDown Error %s"), *Name);
			return;
		}
		InputProcessor->MouseButtonDownEventObserverCount -= 1;
	}
}

void UGInputSubsystem::UnObserveMouseButtonUp(const FString& Name)
{
	if(InputProcessor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("UGInputSubsystem::UnObserveMouseButtonDown %s"), *Name);
		if(InputProcessor->MouseButtonUpEventObserverCount == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UGInputSubsystem::UnObserveMouseButtonDown Error %s"), *Name);
			return;
		}
		InputProcessor->MouseButtonUpEventObserverCount -= 1;
	}
}