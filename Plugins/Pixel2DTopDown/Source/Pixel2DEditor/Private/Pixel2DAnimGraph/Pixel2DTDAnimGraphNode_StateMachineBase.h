//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraphNode_Base.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "Pixel2DTDAnimNode_StateMachine.h"
#include "Pixel2DTDAnimStateMachineGraph.h"
#include "Pixel2DTDAnimGraphNode_StateMachineBase.generated.h"

class INameValidatorInterface;

UCLASS(Abstract)
class PIXEL2DEDITORTD_API UPixel2DTDAnimGraphNode_StateMachineBase : public UPixel2DTDAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	// Editor state machine representation
	UPROPERTY()
	class UPixel2DTDAnimStateMachineGraph* EditorStateMachineGraph;

	// UEdGraphNode interface
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PostPlacedNewNode() override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	virtual void JumpToDefinition() const override;
	virtual void DestroyNode() override;
	virtual void PostPasteNode() override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;
	virtual void OnRenameNode(const FString& NewName) override;
	// End of UEdGraphNode interface

	// UPixel2DTDAnimGraphNode_Base interface
	virtual FString GetNodeCategory() const override;
	// End of UPixel2DTDAnimGraphNode_Base interface

	//  @return the name of this state machine
	FString GetStateMachineName();

	// Interface for derived classes to implement
	virtual FPixel2DTDAnimNode_StateMachine& GetNode() PURE_VIRTUAL(UPixel2DTDAnimGraphNode_StateMachineBase::GetNode, static FPixel2DTDAnimNode_StateMachine Dummy; return Dummy;);
	// End of my interface

private:
	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedFullTitle;
};
