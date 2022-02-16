//Polyart Studio 2021
 

#include "Pixel2DTDAnimNode_Root.h"

/////////////////////////////////////////////////////
// FPixel2DTDAnimNode_Root

FPixel2DTDAnimNode_Root::FPixel2DTDAnimNode_Root()
{
}

void FPixel2DTDAnimNode_Root::Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context)
{
	FPixel2DTDAnimNode_Base::Initialize_AnyThread(Context);
	Result.Initialize(Context);
}

void FPixel2DTDAnimNode_Root::Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context)
{
	EvaluateGraphExposedInputs.Execute(Context);
	Result.Update(Context);
}

void FPixel2DTDAnimNode_Root::Evaluate_AnyThread(FPixel2DTDPoseContext& Output)
{
	Result.Evaluate(Output);
}