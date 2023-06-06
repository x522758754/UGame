//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraphSchema_K2.h"
#include "Pixel2DTDAnimStateConduitGraphSchema.generated.h"

//@TODO: Should this derive from AnimationTransitionSchema (with appropriate suppression of state-based queries)
UCLASS(MinimalAPI)
class UPixel2DTDAnimStateConduitGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_UCLASS_BODY()

	// UEdGraphSchema interface
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual bool CanDuplicateGraph(UEdGraph* InSourceGraph) const override { return false; }
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const override;
	virtual bool DoesSupportEventDispatcher() const	override { return false; }
	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; }
	virtual void HandleGraphBeingDeleted(UEdGraph& GraphBeingRemoved) const override;
	// End of UEdGraphSchema interface
};
