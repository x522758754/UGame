#pragma once

#include "CoreMinimal.h"
#include "GEventSubsystem.h"
#include "GEventDef.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GEventBasic.generated.h"

UCLASS()
class RUNNER_API UGEventBasicFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	template <typename ...Args>
	static void Dispatch(EGEventType Event, Args... args){
		if(UGEventSubsystem* Subsystem = UGEventSubsystem::Get())
		{
			Subsystem->DispatchEvent(static_cast<int>(Event), std::forward<Args>(args)...);
		}
	}
};