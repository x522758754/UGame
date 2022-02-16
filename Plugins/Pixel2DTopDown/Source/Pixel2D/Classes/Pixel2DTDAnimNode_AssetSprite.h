//Polyart Studio 2021
 


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Pixel2DTDAnimNode_Base.h"
#include "Pixel2DTDAnimNotifyEvent.h"
#include "Pixel2DTDAnimNode_AssetSprite.generated.h"

class UPaperFlipbook;

USTRUCT()
struct PIXEL2DTD_API FPixel2DTDAnimNode_AssetSprite : public FPixel2DTDAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	const static int32 MaxNotifySlots = 20;

public:
	UPROPERTY()
	UPaperFlipbook * AssetFlipbook;

	UPROPERTY()
	TArray<FPixel2DTDAnimNotifyEvent> NotifyEvents;

	float ElapsedTime;
	float RelativeTimeInFlipbook = 0;

	/** Get the animation asset associated with the node, derived classes should implement this */
	virtual UPaperFlipbook* GetAnimAsset() { return AssetFlipbook; }

	// FPixel2DTDAnimNode_Base interface
	virtual void Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context);
	virtual void UpdateAssetPlayer(const FPixel2DTDAnimationUpdateContext& Context);
	virtual void Evaluate_AnyThread(FPixel2DTDPoseContext& Output) override;
	// End of FPixel2DTDAnimNode_Base interface

	// Functions to report data to getters
	virtual float GetCurrentAssetLength();
	virtual FName GetNotifyName(int32 NotifyIndex) {
		return NotifyEvents[NotifyIndex].NotifyName;
	}

	virtual float UpdateNotifyPosition(int32 NotifyIndex, int32 SlotIndex);
	virtual bool IsNotifyInRange(int32 SlotIndex);
};