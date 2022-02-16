//Polyart Studio 2021


#include "Pixel2DEditorModule.h"
#include "Editor.h"
#include "Modules/ModuleManager.h"

// Editor settings
#include "ISettingsModule.h"
#include "Pixel2DTDRuntimeSettings.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"

#include "CoreMinimal.h"

#include "Editor/AnimationBlueprintEditor/Private/AnimationGraphFactory.h"
#include "Animation/AnimInstance.h"
#include "Editor/AnimationBlueprintEditor/Private/AnimationBlueprintEditor.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_Event.h"
#include "Engine/MemberReference.h"
#include "Kismet2/KismetEditorUtilities.h"

#include "Pixel2DStyle.h"
#include "Pixel2DAnimGraphFactory.h"
#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDAnimBlueprint.h"
#include "Pixel2DAnimBlueprintCompilerContext.h"
#include "Pixel2DAnimAssetTypeActions.h"

#include "LevelEditor.h"
#include "Pixel2DAnimAssetBrowser.h"

#define LOCTEXT_NAMESPACE "Pixel2DEditor"

//////////////////////////////////////////////////////////////////////////
// FPixel2DEditor

class FPixel2DEditorModule : public IPixel2DAnimEditorModule, public IBlueprintCompiler
{
public:
	FPixel2DEditorModule()
	{}

private:
	TSharedPtr<FPixel2DAnimGraphNodeFactory> SpriteAnimGraphNodeFactory;
	TSharedPtr<FPixel2DAnimGraphPinFactory> SpriteAnimGraphPinFactory;
	TSharedPtr<FPixel2DAnimGraphPinConnectionFactory> SpriteAnimGraphPinConnectionFactory;

	EAssetTypeCategories::Type SpriteAnimAssetCategoryBit;

	/** All created asset type actions.  Cached here so that we can unregister them during shutdown. */
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

public:
	virtual void StartupModule() override
	{
		SpriteAnimGraphNodeFactory = MakeShareable(new FPixel2DAnimGraphNodeFactory());
		FEdGraphUtilities::RegisterVisualNodeFactory(SpriteAnimGraphNodeFactory);

		SpriteAnimGraphPinFactory = MakeShareable(new FPixel2DAnimGraphPinFactory());
		FEdGraphUtilities::RegisterVisualPinFactory(SpriteAnimGraphPinFactory);

		SpriteAnimGraphPinConnectionFactory = MakeShareable(new FPixel2DAnimGraphPinConnectionFactory());
		FEdGraphUtilities::RegisterVisualPinConnectionFactory(SpriteAnimGraphPinConnectionFactory);

		// Register widget blueprint compiler we do this no matter what.
		IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
		KismetCompilerModule.GetCompilers().Add(this);

		FKismetCompilerContext::RegisterCompilerForBP(UPixel2DTDAnimBlueprint::StaticClass(), &FPixel2DEditorModule::GetCompilerForWidgetBP);

		// Register asset types
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		SpriteAnimAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Pixel2DTopDown")), LOCTEXT("SpriteAnimTDAssetCategory", "Pixel2D Top-Down"));

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FPixel2DAnimAssetTypeActions(SpriteAnimAssetCategoryBit)));

		FKismetEditorUtilities::RegisterOnBlueprintCreatedCallback(this, UPixel2DTDAnimInstance::StaticClass(), FKismetEditorUtilities::FOnBlueprintCreated::CreateRaw(this, &FPixel2DEditorModule::OnNewBlueprintCreated));


		// REVISIT: WIP
		//// Register the details customizations
		//{
		//	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		//	PropertyModule.RegisterCustomPropertyTypeLayout(FPixel2DTDLayerMember::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPixel2DLayerDetailsCustomization::MakeInstance));

		//	PropertyModule.NotifyCustomizationModuleChanged();
		//}

		//// LayerExtension
		//{
		//	//TSharedPtr<FPixel2DLayerExtension> AxeLayerExtension = MakeShareable<FPixel2DLayerExtension>(new FPixel2DLayerExtension());
		//	FPixel2DLayerExtension* AxeLayerExtension = new FPixel2DLayerExtension();
		//	FPixel2DLayerCommands::Register();
		//	TSharedPtr<class FUICommandList> PluginCommands = MakeShareable(new FUICommandList);

		//	PluginCommands->MapAction(
		//		FPixel2DLayerCommands::Get().OpenPluginWindow,
		//		FExecuteAction::CreateRaw(AxeLayerExtension, &FPixel2DLayerExtension::PluginButtonClicked),
		//		FCanExecuteAction());

		//	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		//	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(AxeLayerExtension, &FPixel2DLayerExtension::AddMenuExtension));

		//	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		//	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
		//}

		RegisterSettings();

		// Register slate style overrides
		FPixel2DStyle::Initialize();
	}

	virtual void ShutdownModule() override
	{
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

		UnregisterSettings();

		// Unregister slate style overrides
		FPixel2DStyle::Shutdown();
	}

	static TSharedPtr<FKismetCompilerContext> GetCompilerForWidgetBP(UBlueprint* BP, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompileOptions)
	{
		return TSharedPtr<FKismetCompilerContext>(new FPixel2DAnimBlueprintCompilerContext(CastChecked<UPixel2DTDAnimBlueprint>(BP), InMessageLog, InCompileOptions));
	}

	// IBlueprintCompiler interface
	bool CanCompile(const UBlueprint* Blueprint) override
	{
		return Cast<UPixel2DTDAnimBlueprint>(Blueprint) != nullptr;
	}

	void PreCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions) override
	{}

	void Compile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions, FCompilerResultsLog& Results) override
	{
		if (UPixel2DTDAnimBlueprint* SpriteAnimBlueprint = CastChecked<UPixel2DTDAnimBlueprint>(Blueprint))
		{
			FPixel2DAnimBlueprintCompilerContext Compiler(SpriteAnimBlueprint, Results, CompileOptions);
			Compiler.Compile();
			check(Compiler.NewClass);
		}
	}

	void PostCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions) override
	{}

	TSharedRef<class FWorkflowTabFactory> CreateAnimationAssetBrowserTabFactory(const TSharedRef<class FWorkflowCentricApplication>& InHostingApp, bool bInShowHistory) const override
	{
		return MakeShareable(new FPixel2DAnimAssetBrowserSummoner(InHostingApp, bInShowHistory));
	}

private:
	void RegisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "Pixel2D TopDown",
				LOCTEXT("RuntimeSettingsNameTD", "Pixel 2D TopDown"),
				LOCTEXT("RuntimeSettingsDescriptionTD", "Configure Pixel2D TopDown plugin"),
				GetMutableDefault<UPixel2DTDRuntimeSettings>());
		}
	}

	void UnregisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "Pixel2D TopDown");
		}
	}

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}
   
	void OnNewBlueprintCreated(UBlueprint* InBlueprint)
	{
		if (ensure(InBlueprint->UbergraphPages.Num() > 0))
		{
			UEdGraph* EventGraph = InBlueprint->UbergraphPages[0];

			UK2Node_Event* NewEventNode = nullptr;
			FMemberReference EventReference;
			EventReference.SetExternalMember(FName("BlueprintUpdateAnimation"), UPixel2DTDAnimInstance::StaticClass());

			// Add the event
			NewEventNode = NewObject<UK2Node_Event>(EventGraph);
			NewEventNode->EventReference = EventReference;

			// add update event graph
			NewEventNode->bOverrideFunction = true;
			NewEventNode->CreateNewGuid();
			NewEventNode->PostPlacedNewNode();
			NewEventNode->SetFlags(RF_Transactional);
			NewEventNode->AllocateDefaultPins();
			NewEventNode->bCommentBubblePinned = true;
			NewEventNode->bCommentBubbleVisible = true;
			NewEventNode->NodePosY = 0;
			UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);

			EventGraph->AddNode(NewEventNode);
			NewEventNode->MakeAutomaticallyPlacedGhostNode();
		}
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FPixel2DEditorModule, Pixel2DEditorTD);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
