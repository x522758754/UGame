//Polyart Studio 2021


#include "Pixel2DEditorTileMapModule.h"
#include "Editor.h"
#include "EditorModeRegistry.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"

#include "CoreMinimal.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"

#include "AssetToolsModule.h"
#include "IAssetTypeActions.h"
#include "Pixel2DTileMapAssetTypeActions.h"
#include "Pixel2DMapStyle.h"
#include "Pixel2DTileMapEditorCommands.h"
#include "Pixel2DTileMapAssetBroker.h"
#include "Pixel2DTDTileMapComponent.h"
#include "Pixel2DTileMapDetailsCustomization.h"
#include "Pixel2DEdModeTileMap.h"
#include "Pixel2DTDTileMapActor.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEditorPixelTileMap, Log, All);

DEFINE_LOG_CATEGORY(LogEditorPixelTileMap)

#define LOCTEXT_NAMESPACE "Pixel2DEditorTileMap"


//////////////////////////////////////////////////////////////////////////
// FPixel2DEditorPawnSensingModule

class FPixel2DEditorTileMap : public IPixel2DEditorTileMapModule
{
public:
	FPixel2DEditorTileMap()
		: Pixel2DAssetCategoryBit(EAssetTypeCategories::Misc)
	{}

private:
	/** All created asset type actions.  Cached here so that we can unregister them during shutdown. */
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type Pixel2DAssetCategoryBit;
	TSharedPtr<IComponentAssetBroker> Pixel2DTileMapBroker;

public:
	virtual void StartupModule() override
	{
		// Register slate style overrides
		FPixel2DMapStyle::Initialize();

		// Register commands
		FPixel2DTileMapEditorCommands::Register();


		// Register asset types
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		Pixel2DAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Pixel2DTopDown")), LOCTEXT("Pixel2DTDAssetCategory", "Pixel2D Top-Down"));

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FPixel2DTileMapAssetTypeActions(Pixel2DAssetCategoryBit)));


		Pixel2DTileMapBroker = MakeShareable(new FPixel2DTileMapAssetBroker);
		FComponentAssetBrokerage::RegisterBroker(Pixel2DTileMapBroker, UPixel2DTDTileMapComponent::StaticClass(), true, true);

		// Register the details customizations
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.RegisterCustomClassLayout(APixel2DTDTileMapActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPixel2DTileMapDetailsCustomization::MakeInstance));
			PropertyModule.RegisterCustomClassLayout(UPixel2DTDTileMapComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPixel2DTileMapDetailsCustomization::MakeInstance));
			PropertyModule.RegisterCustomClassLayout(UPixel2DTDTileMap::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPixel2DTileMapDetailsCustomization::MakeInstance));
			PropertyModule.NotifyCustomizationModuleChanged();
		}

		// Register the editor modes
		FEditorModeRegistry::Get().RegisterMode<FPixel2DEdModeTileMap>(
			FPixel2DEdModeTileMap::EM_TileMap,
			LOCTEXT("Pixel2DTDTileMapEditMode", "Pixel2D TopDown Tile Map Editor"),
			FSlateIcon(),
			false);
	}

	virtual void ShutdownModule() override
	{

		if (UObjectInitialized())
		{
			FComponentAssetBrokerage::UnregisterBroker(Pixel2DTileMapBroker);

			// Unregister the editor modes
			FEditorModeRegistry::Get().UnregisterMode(FPixel2DEdModeTileMap::EM_TileMap);
		}
		// Unregister all the asset types that we registered
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}
		CreatedAssetTypeActions.Empty();

		// Unregister slate style overrides
		FPixel2DMapStyle::Shutdown();
	}

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FPixel2DEditorTileMap, Pixel2DEditorTileMapTD);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
