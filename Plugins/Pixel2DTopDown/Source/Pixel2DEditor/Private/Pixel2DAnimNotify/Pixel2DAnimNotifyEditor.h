//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/Commands/UICommandList.h"

#include "Pixel2DTDAnimGraphNode_AssetSprite.h"


#define LOCTEXT_NAMESPACE "FPixel2DNotifyEditor"

class SPixel2DAnimNotifyViewport;
class UPaperFlipbookComponent;

class FPixel2DAnimNotifyEditor : public TSharedFromThis<FPixel2DAnimNotifyEditor>
{
public:
	FPixel2DAnimNotifyEditor();

	static const FName Pixel2DAnimNotifyEditorTabName; 

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	void InitNotifyEditor(UPixel2DTDAnimGraphNode_AssetSprite* InitNode);

	UPixel2DTDAnimGraphNode_AssetSprite * GetEditedNode() const { return EditedNode; };
	UPaperFlipbook * GetEditedFlipbook() const { return EditedNode->GetAnimationAsset(); };


private:
	UPaperFlipbookComponent * GetPreviewComponent() const;
	void UpdateSelectedFrame(int NewIndex) const;

protected:
	bool bRegistered = false;

	UPixel2DTDAnimGraphNode_AssetSprite * EditedNode;
	int32 CurrentNotifySlot;

	TSharedPtr<SPixel2DAnimNotifyViewport> ViewportPtr;

public:
	TSharedRef<FUICommandList> NotifyEditorCommandList;

private:
	void OnSelectionChanged(int32 SlotIndex, int32 NewIndex);
	void OnMoveNotifyEvent(int32 NotifyIndex);
};


#undef LOCTEXT_NAMESPACE


