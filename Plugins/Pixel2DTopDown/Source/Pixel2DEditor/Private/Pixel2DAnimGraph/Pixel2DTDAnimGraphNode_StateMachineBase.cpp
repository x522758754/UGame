//Polyart Studio 2021


#include "Pixel2DTDAnimGraphNode_StateMachineBase.h"
#include "EdGraph/EdGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DAnimBlueprintUtils.h"

#include "Kismet2/Kismet2NameValidators.h"

#include "Pixel2DTDAnimGraph.h"
#include "Pixel2DTDAnimStateMachineGraph.h"
#include "Pixel2DTDAnimStateMachineSchema.h"
#include "Pixel2DTDAnimGraphNode_StateMachine.h"
#include "Kismet2/KismetEditorUtilities.h"

/////////////////////////////////////////////////////
// FPixel2DAnimStateMachineNodeNameValidator

class FPixel2DAnimStateMachineNodeNameValidator : public FStringSetNameValidator
{
public:
	FPixel2DAnimStateMachineNodeNameValidator(const UPixel2DTDAnimGraphNode_StateMachineBase* InStateMachineNode)
		: FStringSetNameValidator(FString())
	{
		// REVISIT
	}
};

/////////////////////////////////////////////////////
// UPixel2DTDAnimGraphNode_StateMachineBase

#define LOCTEXT_NAMESPACE "UPixel2DTDAnimGraphNode_StateMachineBase"

UPixel2DTDAnimGraphNode_StateMachineBase::UPixel2DTDAnimGraphNode_StateMachineBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FLinearColor UPixel2DTDAnimGraphNode_StateMachineBase::GetNodeTitleColor() const
{
	return FLinearColor(0.8f, 0.8f, 0.8f);
}

FText UPixel2DTDAnimGraphNode_StateMachineBase::GetTooltipText() const
{
	return LOCTEXT("SpriteStateMachineTooltip", "Sprite Animation State Machine");
}

FText UPixel2DTDAnimGraphNode_StateMachineBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if ((TitleType == ENodeTitleType::MenuTitle || TitleType == ENodeTitleType::ListView) && (EditorStateMachineGraph == nullptr))
	{
		return LOCTEXT("AddNewStateMachine", "Add New Sprite State Machine...");
	}
	else if (EditorStateMachineGraph == nullptr)
	{
		if (TitleType == ENodeTitleType::FullTitle)
		{
			return LOCTEXT("NullStateMachineFullTitle", "Error: No Graph\nState Machine");
		}
		else
		{
			return LOCTEXT("ErrorNoGraph", "Error: No Graph");
		}
	}
	else if (TitleType == ENodeTitleType::FullTitle)
	{
		if (CachedFullTitle.IsOutOfDate(this))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("Title"), FText::FromName(EditorStateMachineGraph->GetFName()));
			// FText::Format() is slow, so we cache this to save on performance
			CachedFullTitle.SetCachedText(FText::Format(LOCTEXT("StateMachineFullTitle", "{Title}\nSprite State Machine"), Args), this);
		}
		return CachedFullTitle;
	}

	return FText::FromName(EditorStateMachineGraph->GetFName());
}

FString UPixel2DTDAnimGraphNode_StateMachineBase::GetNodeCategory() const
{
	return TEXT("Sprite State Machines");
}

void UPixel2DTDAnimGraphNode_StateMachineBase::PostPlacedNewNode()
{
	// Create a new animation graph
	check(EditorStateMachineGraph == NULL);
	EditorStateMachineGraph = CastChecked<UPixel2DTDAnimStateMachineGraph>(FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UPixel2DTDAnimStateMachineGraph::StaticClass(), UPixel2DTDAnimStateMachineSchema::StaticClass()));
	check(EditorStateMachineGraph);
	EditorStateMachineGraph->OwnerAnimGraphNode = this;

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(EditorStateMachineGraph, NameValidator, TEXT("New Sprite State Machine"));

	// Initialize the anim graph
	const UEdGraphSchema* Schema = EditorStateMachineGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*EditorStateMachineGraph);

	// Add the new graph as a child of our parent graph
	UEdGraph* ParentGraph = GetGraph();

	if (ParentGraph->SubGraphs.Find(EditorStateMachineGraph) == INDEX_NONE)
	{
		ParentGraph->Modify();
		ParentGraph->SubGraphs.Add(EditorStateMachineGraph);
	}
}

UObject* UPixel2DTDAnimGraphNode_StateMachineBase::GetJumpTargetForDoubleClick() const
{
	// Open the state machine graph
	return EditorStateMachineGraph;
}

void UPixel2DTDAnimGraphNode_StateMachineBase::JumpToDefinition() const
{
	if (UObject* HyperlinkTarget = GetJumpTargetForDoubleClick())
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(HyperlinkTarget);
	}
}

void UPixel2DTDAnimGraphNode_StateMachineBase::DestroyNode()
{
	UEdGraph* GraphToRemove = EditorStateMachineGraph;

	EditorStateMachineGraph = NULL;
	Super::DestroyNode();

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = GetBlueprint();
		GraphToRemove->Modify();
		FPixel2DAnimBlueprintUtils::RemoveGraph(Blueprint, GraphToRemove, EGraphRemoveFlags::Recompile);
	}
}

void UPixel2DTDAnimGraphNode_StateMachineBase::PostPasteNode()
{
	Super::PostPasteNode();

	// Add the new graph as a child of our parent graph
	UEdGraph* ParentGraph = GetGraph();

	if (ParentGraph->SubGraphs.Find(EditorStateMachineGraph) == INDEX_NONE)
	{
		ParentGraph->SubGraphs.Add(EditorStateMachineGraph);
	}

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(EditorStateMachineGraph, NameValidator, EditorStateMachineGraph->GetName());

	//restore transactional flag that is lost during copy/paste process
	EditorStateMachineGraph->SetFlags(RF_Transactional);
}

FString UPixel2DTDAnimGraphNode_StateMachineBase::GetStateMachineName()
{
	return (EditorStateMachineGraph != NULL) ? *(EditorStateMachineGraph->GetName()) : TEXT("(null)");
}

TSharedPtr<class INameValidatorInterface> UPixel2DTDAnimGraphNode_StateMachineBase::MakeNameValidator() const
{
	return MakeShareable(new FPixel2DAnimStateMachineNodeNameValidator(this));
}

void UPixel2DTDAnimGraphNode_StateMachineBase::OnRenameNode(const FString& NewName)
{
	FBlueprintEditorUtils::RenameGraph(EditorStateMachineGraph, NewName);
}

#undef LOCTEXT_NAMESPACE
