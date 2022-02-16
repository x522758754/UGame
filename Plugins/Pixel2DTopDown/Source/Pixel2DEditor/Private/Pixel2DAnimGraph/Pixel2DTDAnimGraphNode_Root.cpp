//Polyart Studio 2021


#include "Pixel2DTDAnimGraphNode_Root.h"
#include "GraphEditorSettings.h"


/////////////////////////////////////////////////////
// FPixel2DTDPoseLinkMappingRecord

void FPixel2DTDPoseLinkMappingRecord::PatchLinkIndex(uint8* DestinationPtr, int32 LinkID, int32 SourceLinkID) const
{
	checkSlow(IsValid());

	DestinationPtr = ChildProperty->ContainerPtrToValuePtr<uint8>(DestinationPtr);

	if (ChildPropertyIndex != INDEX_NONE)
	{
		FArrayProperty* ArrayProperty = CastFieldChecked<FArrayProperty>(ChildProperty);

		FScriptArrayHelper ArrayHelper(ArrayProperty, DestinationPtr);
		check(ArrayHelper.IsValidIndex(ChildPropertyIndex));

		DestinationPtr = ArrayHelper.GetRawPtr(ChildPropertyIndex);
	}

	// Check to guard against accidental infinite loops
	check((LinkID == INDEX_NONE) || (LinkID != SourceLinkID));

	// Patch the pose link
	FPixel2DTDPoseLinkBase& PoseLink = *((FPixel2DTDPoseLinkBase*)DestinationPtr);
	PoseLink.LinkID = LinkID;
	PoseLink.SourceLinkID = SourceLinkID;
}

/////////////////////////////////////////////////////
// UPixel2DTDAnimGraphNode_Root

#define LOCTEXT_NAMESPACE "UPixel2DTDAnimGraphNode_Root"

UPixel2DTDAnimGraphNode_Root::UPixel2DTDAnimGraphNode_Root(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FLinearColor UPixel2DTDAnimGraphNode_Root::GetNodeTitleColor() const
{
	return GetDefault<UGraphEditorSettings>()->ResultNodeTitleColor;
}

FText UPixel2DTDAnimGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("SpriteAnimGraphNodeRoot_Title", "Final Pixel2D Top-Down Flipbook");
}

FText UPixel2DTDAnimGraphNode_Root::GetTooltipText() const
{
	return LOCTEXT("SpriteAnimGraphNodeRoot_Tooltip", "Wire the final sprite animation into this node");
}

bool UPixel2DTDAnimGraphNode_Root::IsSinkNode() const
{
	return true;
}

void UPixel2DTDAnimGraphNode_Root::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// Intentionally empty. This node is auto-generated when a new graph is created.
}

#undef LOCTEXT_NAMESPACE
