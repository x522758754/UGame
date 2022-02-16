//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node_Event.h"
#include "BlueprintActionFilter.h"
#include "UK2Node_PixelTDEvent.generated.h"

class FBlueprintActionDatabaseRegistrar;

UCLASS(MinimalAPI)
class UK2Node_PixelTDEvent : public UK2Node_Event
{
	GENERATED_BODY()
public:

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual bool IsActionFilteredOut(FBlueprintActionFilter const& Filter) override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// end of UK2Node interface

	// Cached node title
	UPROPERTY()
	FText CachedTitle;
};