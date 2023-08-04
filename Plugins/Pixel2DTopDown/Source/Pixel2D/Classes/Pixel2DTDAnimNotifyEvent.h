//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"
#include "Misc/MemStack.h"
#include "Animation/AnimTypes.h"
#include "Pixel2DTDAnimNotifyEvent.generated.h"

/**
* Indicates whether an anim notify can be triggered.
*/
USTRUCT(BlueprintType)
struct FPixel2DTDAnimNotifyEvent : public FAnimNotifyEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool bEnabled = false;

	UPROPERTY()
	bool bTriggered = false;

	/** At what percentage of the total execution time the event can trigger*/
	UPROPERTY()
	float NormalisedTriggerOffset;

	UPROPERTY()
	FGameplayTag EventTag;

	bool CanTrigger(float ElapsedTime, float TotalTime);
};
