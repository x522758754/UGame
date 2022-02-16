//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimNode_Base.h"
#include "Pixel2DTDAnimNode_Root.generated.h"

// Root node of an animation tree (sink)
USTRUCT(BlueprintInternalUseOnly)
struct PIXEL2DTD_API FPixel2DTDAnimNode_Root : public FPixel2DTDAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FPixel2DTDPoseLink Result;

public:
	FPixel2DTDAnimNode_Root();

	// FPixel2DTDAnimNode_Base interface
	virtual void Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPixel2DTDPoseContext& Output) override;
	// End of FPixel2DTDAnimNode_Base interface
};
