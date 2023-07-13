// Fill out your copyright notice in the Description page of Project Settings.

#include "Event/GEventSubsystem.h"
#include "System/GGameInstance.h"

TWeakObjectPtr<UGEventSubsystem> UGEventSubsystem::s_Instance;
UGEventSubsystem* UGEventSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGEventSubsystem>();
	}
	return s_Instance.Get();
}

void UGEventSubsystem::UnObserve(int64 ObserveGuid)
{
	int Event = static_cast<int>(ObserveGuid >> 32);
	int32 Index = static_cast<int32>(ObserveGuid & 0x00000000ffffffff);
	TSharedPtr<FGEventDispatcherAbstract>* Value = EventDispatcherMap.Find(Event);
	if(Value == nullptr || Value->Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGEventSubsystem::UnObserve Event=%d, Index=%d"), static_cast<int>(Event), Index);
		return;
	}
	//UE_LOG(LogTemp, Log, TEXT("UGEventSubsystem::UnObserve Event=%d, Index=%d"), static_cast<int>(Event), Index);
	Value->Get()->RemoveObserver(Index);
}
