// Fill out your copyright notice in the Description page of Project Settings.

#include "Event/GEventObserverHolder.h"
#include "Event/GEventSubsystem.h"


FGEventObserverHolder::~FGEventObserverHolder()
{
	UnObserver();
}

void FGEventObserverHolder::AddGuid(const int64& Guid)
{
	ObserverGuidArray.Add(Guid);
}

void FGEventObserverHolder::UnObserver()
{
	if(UGEventSubsystem* Subsystem = UGEventSubsystem::Get())
	{
		for(const int64& Guid : ObserverGuidArray)
		{
			Subsystem->UnObserve(Guid);
		}
	}
	ObserverGuidArray.Empty();
}
