//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "KismetNodes/SGraphNodeK2Composite.h"

class UEdGraph;

class SPixel2DGraphNodeStateMachineInstance : public SGraphNodeK2Composite
{
public:
	SLATE_BEGIN_ARGS(SPixel2DGraphNodeStateMachineInstance) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UPixel2DTDAnimGraphNode_StateMachineBase* InNode);
protected:
	// SGraphNodeK2Composite interface
	virtual UEdGraph* GetInnerGraph() const override;
	// End of SGraphNodeK2Composite interface
};
