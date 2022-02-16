//Polyart Studio 2021


#include "UK2Node_PixelTDEvent.h"
#include "Pixel2DTDAnimBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDAnimGraphSchema.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintEventNodeSpawner.h"
#include "Pixel2DTDAnimGraphNode_AssetPlayerBase.h"
#include "Pixel2DTDAnimGraphNode_AssetSprite.h"

#define LOCTEXT_NAMESPACE "PixelEvent"

FText UK2Node_PixelTDEvent::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return CachedTitle;
}

bool UK2Node_PixelTDEvent::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return true;
}

void UK2Node_PixelTDEvent::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// First cache the available functions for getters
	UClass* ActionKey = GetClass();
	const UPixel2DTDAnimBlueprint* AnimBlueprint = Cast<UPixel2DTDAnimBlueprint>(ActionRegistrar.GetActionKeyFilter());
	if (AnimBlueprint && ActionRegistrar.IsOpenForRegistration(AnimBlueprint))
	{
		UClass* BPClass = *AnimBlueprint->ParentClass;
		while (BPClass && !BPClass->HasAnyClassFlags(CLASS_Native))
		{
			BPClass = BPClass->GetSuperClass();
		}

		if (BPClass)
		{
			auto UiSpecOverride = [](const FBlueprintActionContext& /*Context*/, const IBlueprintNodeBinder::FBindingSet& Bindings, FBlueprintActionUiSpec* UiSpecOut, FText Title)
			{
				UiSpecOut->MenuName = Title;
			};

			TArray<UPixel2DTDAnimGraphNode_AssetPlayerBase*> AssetPlayerNodes;
			FBlueprintEditorUtils::GetAllNodesOfClass(AnimBlueprint, AssetPlayerNodes);

			for (UPixel2DTDAnimGraphNode_Base* AssetNode : AssetPlayerNodes)
			{
				// Should always succeed

				if (UPixel2DTDAnimGraphNode_AssetSprite* AssetSprite = Cast<UPixel2DTDAnimGraphNode_AssetSprite>(AssetNode))
				{
					auto PostSpawnSetupLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode, FName NotifyName)
					{
						UK2Node_PixelTDEvent* ActorRefNode = CastChecked<UK2Node_PixelTDEvent>(NewNode);
						ActorRefNode->EventReference.SetExternalMember(ActorRefNode->CustomFunctionName, UPixel2DTDAnimInstance::StaticClass());
						ActorRefNode->CachedTitle = FText::FromName(NotifyName);
					};

					for (int i = 0; i < AssetSprite->NotifyEventsNum(); i++)
					{
						FName NotifyName = AssetSprite->GetNotifyName(i);
						if (AssetSprite->IsNotifyEnabled(i))
						{
							UBlueprintNodeSpawner* NodeSpawner = UBlueprintEventNodeSpawner::Create(UK2Node_PixelTDEvent::StaticClass(), NotifyName);
							NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(PostSpawnSetupLambda, NotifyName);
							ActionRegistrar.AddBlueprintAction(AnimBlueprint, NodeSpawner);
						}
					}
				}
			}
		}
	}
}

bool UK2Node_PixelTDEvent::IsActionFilteredOut(FBlueprintActionFilter const& Filter)
{
	return false;
}

#undef LOCTEXT_NAMESPACE
