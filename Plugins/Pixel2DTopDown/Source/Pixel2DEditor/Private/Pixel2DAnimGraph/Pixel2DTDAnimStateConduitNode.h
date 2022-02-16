//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimStateNodeBase.h"
#include "Pixel2DTDAnimStateConduitNode.generated.h"

class UEdGraph;
class UEdGraphPin;

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateConduitNode : public UPixel2DTDAnimStateNodeBase
{
	GENERATED_UCLASS_BODY()
public:

	// The transition graph for this conduit; it's a logic graph, not an animation graph
	UPROPERTY()
	class UEdGraph* BoundGraph;

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanDuplicateNode() const override { return true; }
	virtual void PostPasteNode() override;
	virtual void PostPlacedNewNode() override;
	virtual void DestroyNode() override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UAnimStateNodeBase Interface
	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;
	virtual FString GetStateName() const override;
	virtual void GetTransitionList(TArray<class UPixel2DTDAnimStateTransitionNode*>& OutTransitions, bool bWantSortedList = true) override;
	virtual FString GetDesiredNewNodeName() const override;
	virtual UEdGraph* GetBoundGraph() const override { return BoundGraph; }
	virtual void ClearBoundGraph() override { BoundGraph = nullptr; }
	//~ End UAnimStateNodeBase Interface
};
