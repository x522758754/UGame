//Polyart Studio 2021


#include "Pixel2DAnimAssetTypeActions.h"
#include "Pixel2DAnimBlueprintEditor.h"
#include "Pixel2DTDAnimBlueprint.h"

#include "Pixel2DTDAnimBlueprintFactory.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

//////////////////////////////////////////////////////////////////////////
// FPixel2DAnimAssetTypeActions

FPixel2DAnimAssetTypeActions::FPixel2DAnimAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FPixel2DAnimAssetTypeActions::GetName() const
{
	return LOCTEXT("FSpriteAnimAssetTypeActionsName", "Sprite Anim");
}

FColor FPixel2DAnimAssetTypeActions::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FPixel2DAnimAssetTypeActions::GetSupportedClass() const
{
	return UPixel2DTDAnimBlueprint::StaticClass();
}

void FPixel2DAnimAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UPixel2DTDAnimBlueprint* SpriteBP = Cast<UPixel2DTDAnimBlueprint>(*ObjIt))
		{
			const bool bBringToFrontIfOpen = true;
			if (IAssetEditorInstance* EditorInstance = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(SpriteBP, bBringToFrontIfOpen))
			{
				EditorInstance->FocusWindow(SpriteBP);
			}
			else
			{
				TSharedRef<FPixel2DAnimBlueprintEditor > NewSpriteAnimBlueprintEditor(new FPixel2DAnimBlueprintEditor());
				NewSpriteAnimBlueprintEditor->InitSpriteAnimBlueprintEditor(Mode, EditWithinLevelEditor, SpriteBP);
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadCorruptAnimBlueprint", "The Anim Blueprint could not be loaded because it is corrupt."));
		}
	}
}

uint32 FPixel2DAnimAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

UFactory* FPixel2DAnimAssetTypeActions::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UPixel2DTDAnimBlueprintFactory* AnimBlueprintFactory = NewObject<UPixel2DTDAnimBlueprintFactory>();
	return AnimBlueprintFactory;
}

#undef LOCTEXT_NAMESPACE
