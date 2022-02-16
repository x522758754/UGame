//Polyart Studio 2021
 


#pragma once

#include "CoreMinimal.h"

#include "Pixel2DTDAnimNotifyQueue.generated.h"

struct FPixel2DTDAnimInstanceProxy;
struct FPixel2DTDAnimNotifyEvent;

USTRUCT()
struct FPixel2DTDAnimNotifyEventReference
{
	GENERATED_BODY()

	FPixel2DTDAnimNotifyEventReference()
		: Notify(nullptr)
		, NotifySource(nullptr)
	{}

	FPixel2DTDAnimNotifyEventReference(const FPixel2DTDAnimNotifyEventReference& rhs)
		: Notify(rhs.Notify)
		, NotifySource(rhs.NotifySource)
	{
	}

	FPixel2DTDAnimNotifyEventReference(const FPixel2DTDAnimNotifyEvent* InNotify, const UObject* InNotifySource)
		: Notify(InNotify)
		, NotifySource(InNotifySource)
	{}

	const FPixel2DTDAnimNotifyEvent* GetNotify() const
	{
		return NotifySource ? Notify : nullptr;
	}

	friend bool operator==(const FPixel2DTDAnimNotifyEventReference& Lhs, const FPixel2DTDAnimNotifyEventReference& Rhs)
	{
		return Lhs.Notify == Rhs.Notify;
	}

	friend bool operator==(const FPixel2DTDAnimNotifyEventReference& Lhs, const FPixel2DTDAnimNotifyEvent& Rhs);

private:

	const FPixel2DTDAnimNotifyEvent* Notify;

	UPROPERTY(transient)
	const UObject* NotifySource;
};

USTRUCT()
struct FPixel2DTDAnimNotifyQueue
{
	GENERATED_BODY()

	/** Add notify to queue*/
	void AddAnimNotify(const FPixel2DTDAnimNotifyEvent* Notify, const UObject* NotifySource);

	/** Reset queue & update LOD level */
	void Reset();

	/** Append one queue to another */
	void Append(const FPixel2DTDAnimNotifyQueue& Queue);

	/** Animation Notifies that has been triggered in the latest tick **/
	UPROPERTY(transient)
	TArray<FPixel2DTDAnimNotifyEventReference> AnimNotifies;

	/** Adds the contents of the NewNotifies array to the DestArray (maintaining uniqueness of notify states*/
	void AddAnimNotifiesToDestNoFiltering(const TArray<FPixel2DTDAnimNotifyEventReference>& NewNotifies, TArray<FPixel2DTDAnimNotifyEventReference>& DestArray) const;
};
