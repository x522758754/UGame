//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimStateNodeBase.h"
#include "Pixel2DTDAnimStateNode.generated.h"

class UEdGraph;
class UEdGraphPin;

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateNode : public UPixel2DTDAnimStateNodeBase
{
	GENERATED_UCLASS_BODY()
public:

	// The animation graph for this state
	UPROPERTY()
	class UEdGraph* BoundGraph;

	// Whether or not this state will ALWAYS reset it's state on reentry, regardless of remaining weight
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Events)
	bool bAlwaysResetOnEntry;

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanDuplicateNode() const override { return true; }
	virtual void PostPasteNode() override;
	virtual void PostPlacedNewNode() override;
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPixel2DTDAnimStateNodeBase Interface
	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;
	virtual FString GetStateName() const override;
	virtual void GetTransitionList(TArray<class UPixel2DTDAnimStateTransitionNode*>& OutTransitions, bool bWantSortedList = false) override;
	//~ End UPixel2DTDAnimStateNodeBase Interface

	// @return the pose pin of the state sink node within the anim graph of this state
	PIXEL2DEDITORTD_API UEdGraphPin* GetPoseSinkPinInsideState() const;

public:
	virtual UEdGraph* GetBoundGraph() const override { return BoundGraph; }
	virtual void ClearBoundGraph() override { BoundGraph = nullptr; }
};
