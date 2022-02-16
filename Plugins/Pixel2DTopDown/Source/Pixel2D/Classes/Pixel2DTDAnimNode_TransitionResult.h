//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimNode_Base.h"
#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDAnimNode_TransitionResult.generated.h"

// Root node of a state machine transition graph
USTRUCT(BlueprintInternalUseOnly)
struct PIXEL2DTD_API FPixel2DTDAnimNode_TransitionResult : public FPixel2DTDAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Result, meta = (AlwaysAsPin))
	bool bCanEnterTransition;

	/** Native delegate to use when checking transition */
	FCanTakeTransition NativeTransitionDelegate;

public:
	FPixel2DTDAnimNode_TransitionResult();

	// FPixel2DTDAnimNode_Base interface
	virtual void Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPixel2DTDPoseContext& Output) override;
	// End of FPixel2DTDAnimNode_Base interface
};
