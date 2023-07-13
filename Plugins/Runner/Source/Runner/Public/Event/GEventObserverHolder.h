#pragma once

#include "CoreMinimal.h"
#include "GEventBasic.h"

class RUNNER_API FGEventObserverHolder
{
public:
	virtual ~FGEventObserverHolder();
public:
	void AddGuid(const int64& Guid);
	void UnObserver();

	template <typename ...Args>
	void Observe(EGEventBasic Event, const TFunction<void(Args...)>& Observer){
		AddGuid(
			UGEventSubsystem::Get()->Observe(static_cast<int>(Event), Observer)
		);
	}
private:
	TArray<int64> ObserverGuidArray;
};
