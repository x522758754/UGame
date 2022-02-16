//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Pixel2DTDAnimGraphSchema.h"
#include "Pixel2DTDAnimStateGraphSchema.generated.h"

UCLASS(MinimalAPI)
class UPixel2DTDAnimStateGraphSchema : public UPixel2DTDAnimGraphSchema
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphSchema Interface.
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const override;
	//~ End UEdGraphSchema Interface.
};
