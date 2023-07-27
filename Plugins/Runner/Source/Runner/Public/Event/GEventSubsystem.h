// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "GEventSubsystem.generated.h"


class FGEventDispatcherAbstract
{
public:
	virtual  ~FGEventDispatcherAbstract(){}
	virtual void RemoveObserver(int Index) = 0;
public:
	virtual const FName& GetTypeName() = 0;
};

template <typename ...Args>
class FGEventDispatcher : public FGEventDispatcherAbstract
{
public:
	int AddObserver(const TFunction<void(Args...)>& Observer)
	{
		int Id = Index;
		ObserverMap.Add(Id, Observer);
		Index++;
		return Id;
	}

	virtual void RemoveObserver(int InIndex) override
	{
		ObserverMap.Remove(InIndex);
	}
public:
	virtual const FName& GetTypeName() override {return StaticTypeName();}
	void Invoke(Args&&... args)
	{
		for(auto& KeyValue : ObserverMap)
		{
			KeyValue.Value(std::forward<Args>(args)...);
		}
	}
	
private:
	int Index = 1;
	TMap<int, TFunction<void(Args...)>> ObserverMap;

public:
	static const FName& StaticTypeName()
	{
		if(!s_TypeName.IsValid())
		{
			s_TypeName =  UTF8_TO_TCHAR(typeid(FGEventDispatcher<Args...>).name());
		}
		return s_TypeName;
	}
private:
	static FName s_TypeName;
};

template <typename ...Args>
FName FGEventDispatcher<Args...>::s_TypeName;

/**
 * 
 */
UCLASS()
class RUNNER_API UGEventSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UGEventSubsystem* Get();
protected:
	static TWeakObjectPtr<UGEventSubsystem> s_Instance;
public:
	void ObserveByLua(int Event, bool Observer)
	{
		if(Observer)
		{
			EventLuaObserver.Add(Event);
		}
		else
		{
			EventLuaObserver.Remove(Event);
		}
	}
public:
	template <typename ...Args>
	int64 Observe(int Event, const TFunction<void(Args...)>& Observer)
	{
		TSharedPtr<FGEventDispatcherAbstract>* Value = EventDispatcherMap.Find(Event);
		if(Value == nullptr)
		{
			Value = &EventDispatcherMap.Add(Event, TSharedPtr<FGEventDispatcherAbstract>(new FGEventDispatcher<Args...>()));
		}
		else
		{
			if((*Value)->GetTypeName() != FGEventDispatcher<Args...>::StaticTypeName())
			{
				UE_LOG(LogTemp, Fatal, TEXT("UGEventSubsystem Observe Error(%s != %s)"), *(*Value)->GetTypeName().ToString(), *FGEventDispatcher<Args...>::StaticTypeName().ToString());
				return -1;
			}
		}
		FGEventDispatcher<Args...>* Dispatcher = static_cast<FGEventDispatcher<Args...>*>(Value->Get());
		return Dispatcher->AddObserver(Observer) + (static_cast<int64>(Event) << 32);
	}

	void UnObserve(int64 ObserveGuid);

	template <typename ...Args>
	void DispatchEvent(int Event, Args... args)
	{
		if(EventLuaObserver.Contains(Event))
		{
			//UGLuaFunctions::Call("Framework.EventSystem", "Dispatch", static_cast<int>(Event), std::forward<Args>(args)...);
		}
		TSharedPtr<FGEventDispatcherAbstract>* Value = EventDispatcherMap.Find(Event);
		if(Value == nullptr)
		{
			return;
		}

		const TSharedPtr<FGEventDispatcherAbstract> EventDispatcher = *Value;
		if(EventDispatcher->GetTypeName() != FGEventDispatcher<Args...>::StaticTypeName())
		{
			UE_LOG(LogTemp, Error, TEXT("UGEventSubsystem DispatchEvent Event=%d Error( %s != %s )"),
				static_cast<int>(Event), *EventDispatcher->GetTypeName().ToString(), *FGEventDispatcher<Args...>::StaticTypeName().ToString());
			return;
		}

		FGEventDispatcher<Args...>* Dispatcher = static_cast<FGEventDispatcher<Args...>*>(EventDispatcher.Get());
		Dispatcher->Invoke(std::forward<Args>(args)...);
	}
	
private:
	TMap<int, TSharedPtr<FGEventDispatcherAbstract>> EventDispatcherMap;
	TSet<int> EventLuaObserver;
};



