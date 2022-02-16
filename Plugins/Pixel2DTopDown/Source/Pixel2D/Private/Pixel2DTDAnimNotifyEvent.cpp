//Polyart Studio 2021
 

#include "Pixel2DTDAnimNotifyEvent.h"

bool FPixel2DTDAnimNotifyEvent::CanTrigger(float ElapsedTime, float TotalTime)
{
	if ((!bEnabled) || (bTriggered))
		return false;

	return (ElapsedTime > TotalTime*NormalisedTriggerOffset);
}
