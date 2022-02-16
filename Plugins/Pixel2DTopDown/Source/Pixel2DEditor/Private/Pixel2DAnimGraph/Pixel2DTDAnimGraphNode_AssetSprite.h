//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "Pixel2DTDAnimGraphNode_AssetPlayerBase.h"
#include "Pixel2DTDAnimNode_AssetSprite.h"
#include "Pixel2DTDAnimGraphNode_AssetSprite.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UPaperFlipbook;
class UPixel2DTDAnimNotify;

UCLASS(MinimalAPI)
class UPixel2DTDAnimGraphNode_AssetSprite : public UPixel2DTDAnimGraphNode_AssetPlayerBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FPixel2DTDAnimNode_AssetSprite Node;

	UPROPERTY(VisibleInstanceOnly, Transient, Category = Notify)
	UPixel2DTDAnimNotify * NotifyProxy;

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of UEdGraphNode interface

	// UPixel2DTDAnimGraphNode_Base interface
	virtual EAnimAssetHandlerType SupportsAssetClass(const UClass* AssetClass) const override;
	// End of UPixel2DTDAnimGraphNode_Base interface

	// UK2Node interface
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	// End of UK2Node interface

	// UPixel2DTDAnimGraphNode_AssetPlayerBase interface
	virtual void SetAnimationAsset(UPaperFlipbook* Asset) override;
	// End of UPixel2DTDAnimGraphNode_AssetPlayerBase interface

public:
	virtual UPaperFlipbook * GetAnimationAsset() { return Node.AssetFlipbook; };
	virtual FName            GetNotifyName(int32 NotifyIndex);
	virtual bool             IsNotifyEnabled(int32 NotifyIndex);
	virtual int              NotifyEventsNum() { return Node.NotifyEvents.Num(); }
	virtual TArray<FName>	 NotifyEventsNames();
	virtual UPixel2DTDAnimNotify * GetNotify();

private:
	FText GetNodeTitleForFlipbook(ENodeTitleType::Type TitleType, UPaperFlipbook* Flipbook) const;
	void InitNotifyProxy();

private:
	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTitleTextTable CachedNodeTitles;
};
