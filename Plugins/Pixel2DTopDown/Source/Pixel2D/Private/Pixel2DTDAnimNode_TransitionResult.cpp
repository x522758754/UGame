//Polyart Studio 2021
 

#include "Pixel2DTDAnimNode_TransitionResult.h"

/////////////////////////////////////////////////////
// FPixel2DTDAnimNode_TransitionResult

FPixel2DTDAnimNode_TransitionResult::FPixel2DTDAnimNode_TransitionResult() 
	: bCanEnterTransition(false)
{
}

void FPixel2DTDAnimNode_TransitionResult::Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context)
{
	FPixel2DTDAnimNode_Base::Initialize_AnyThread(Context);
}

void FPixel2DTDAnimNode_TransitionResult::Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context)
{
}

void FPixel2DTDAnimNode_TransitionResult::Evaluate_AnyThread(FPixel2DTDPoseContext& Output)
{
}
