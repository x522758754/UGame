//Polyart Studio 2021
 

#include "Pixel2DTDAnimNotifyQueue.h"
#include "Pixel2DTDAnimNotifyEvent.h"

bool operator==(const FPixel2DTDAnimNotifyEventReference& Lhs, const FPixel2DTDAnimNotifyEvent& Rhs)
{
	if (Lhs.NotifySource && Lhs.Notify)
	{
		return (*(Lhs.Notify)) == Rhs;
	}
	return false;
}

void FPixel2DTDAnimNotifyQueue::AddAnimNotifiesToDestNoFiltering(const TArray<FPixel2DTDAnimNotifyEventReference>& NewNotifies, TArray<FPixel2DTDAnimNotifyEventReference>& DestArray) const
{
	for (const FPixel2DTDAnimNotifyEventReference& NotifyRef : NewNotifies)
	{
		if (const FPixel2DTDAnimNotifyEvent* Notify = NotifyRef.GetNotify())
		{
			Notify->NotifyStateClass ? DestArray.AddUnique(NotifyRef) : DestArray.Add(NotifyRef);
		}
	}
}

void FPixel2DTDAnimNotifyQueue::AddAnimNotify(const FPixel2DTDAnimNotifyEvent* Notify, const UObject* NotifySource)
{
	AnimNotifies.Add(FPixel2DTDAnimNotifyEventReference(Notify, NotifySource));
}

void FPixel2DTDAnimNotifyQueue::Reset()
{
	AnimNotifies.Reset();
}

void FPixel2DTDAnimNotifyQueue::Append(const FPixel2DTDAnimNotifyQueue& Queue)
{
	// we dont just append here - we need to preserve uniqueness for AnimNotifyState instances
	AddAnimNotifiesToDestNoFiltering(Queue.AnimNotifies, AnimNotifies);
}
