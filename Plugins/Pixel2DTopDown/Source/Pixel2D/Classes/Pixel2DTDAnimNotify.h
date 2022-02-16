//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "Pixel2DTDAnimNotifyEvent.h"
#include "Pixel2DTDAnimNotify.generated.h"

USTRUCT()
struct FPixel2DTDAnimNotifyEventArrayElement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = NotifyEvent)
	bool bEnabled = false;

	UPROPERTY(EditAnywhere, Category = NotifyEvent)
	FName NotifyName = FName(TEXT("Pixel2DTD_AnimNotify"));

	UPROPERTY(VisibleAnywhere, Category = NotifyEvent)
	float NormalisedTriggerOffset = 0;
};

// Proxy class for Pixel2DTopDown notifies
UCLASS(Blueprintable, const, hidecategories = Object, collapsecategories)
class PIXEL2DTD_API UPixel2DTDAnimNotify : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = NotifyEvent)
	TArray<FPixel2DTDAnimNotifyEventArrayElement> NotificationEvents;

	TArray<FPixel2DTDAnimNotifyEvent> * NotifyArray;

	void Init(TArray<FPixel2DTDAnimNotifyEvent> & InNotifyArray)
	{
		NotifyArray = &InNotifyArray;
		NotificationEvents.Empty();

		for (auto NotifyEvent : *NotifyArray)
		{
			FPixel2DTDAnimNotifyEventArrayElement elem;
			elem.bEnabled = NotifyEvent.bEnabled;
			elem.NotifyName = NotifyEvent.NotifyName;
			elem.NormalisedTriggerOffset = FMath::RoundHalfFromZero(NotifyEvent.NormalisedTriggerOffset*100)/100;
			NotificationEvents.Push(elem);
		}
	}

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
	{
		NotifyArray->Empty();

		int TotalEvents = NotificationEvents.Num();
		for (int i = 0; i < NotificationEvents.Num(); i++)
		{
			FPixel2DTDAnimNotifyEventArrayElement Notify = NotificationEvents[i];
			if (Notify.NotifyName.IsNone())
			{
				Notify.NotifyName = FName(TEXT("Pixel2D_AnimNotify"));
			}
			FPixel2DTDAnimNotifyEvent NewNotify;
			NewNotify.bEnabled   = Notify.bEnabled;
			NewNotify.NotifyName = Notify.NotifyName;
			NewNotify.NormalisedTriggerOffset = FMath::RoundHalfFromZero(Notify.NormalisedTriggerOffset * 100)/100;
			NotifyArray->Push(NewNotify);
		}
	}
#endif
};