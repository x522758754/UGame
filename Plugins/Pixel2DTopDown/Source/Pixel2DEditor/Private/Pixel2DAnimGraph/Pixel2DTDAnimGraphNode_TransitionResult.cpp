//Polyart Studio 2021


#include "Pixel2DTDAnimGraphNode_TransitionResult.h"
#include "GraphEditorSettings.h"


/////////////////////////////////////////////////////
// UPixel2DTDAnimGraphNode_TransitionResult

#define LOCTEXT_NAMESPACE "UPixel2DTDAnimGraphNode_TransitionResult"

UPixel2DTDAnimGraphNode_TransitionResult::UPixel2DTDAnimGraphNode_TransitionResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FLinearColor UPixel2DTDAnimGraphNode_TransitionResult::GetNodeTitleColor() const
{
	return GetDefault<UGraphEditorSettings>()->ResultNodeTitleColor;
}

FText UPixel2DTDAnimGraphNode_TransitionResult::GetTooltipText() const
{
	return LOCTEXT("TransitionResultTooltip", "This expression is evaluated to determine if the state transition can be taken");
}

FText UPixel2DTDAnimGraphNode_TransitionResult::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Result", "Result");
}

void UPixel2DTDAnimGraphNode_TransitionResult::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// Intentionally empty. This node is auto-generated when a transition graph is created.
}

#undef LOCTEXT_NAMESPACE
