//Polyart Studio 2021


#include "Pixel2DTDAnimStateConduitNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimStateTransitionNode.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/CompilerResultsLog.h"
#include "Pixel2DTDAnimStateConduitGraphSchema.h"
#include "Pixel2DTDAnimGraphNode_TransitionResult.h"
#include "Pixel2DTDAnimTransitionGraph.h"
#include "Pixel2DAnimBlueprintUtils.h"

#define LOCTEXT_NAMESPACE "Pixel2DTDAnimStateConduitNode"

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateConduitNode

UPixel2DTDAnimStateConduitNode::UPixel2DTDAnimStateConduitNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCanRenameNode = true;
}

void UPixel2DTDAnimStateConduitNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
}

void UPixel2DTDAnimStateConduitNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

FText UPixel2DTDAnimStateConduitNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(GetStateName());
}

FText UPixel2DTDAnimStateConduitNode::GetTooltipText() const
{
	return LOCTEXT("ConduitNodeTooltip", "This is a conduit, which allows specification of a predicate condition for an entire group of transitions");
}

FString UPixel2DTDAnimStateConduitNode::GetStateName() const
{
	return (BoundGraph != NULL) ? *(BoundGraph->GetName()) : TEXT("(null)");
}

UEdGraphPin* UPixel2DTDAnimStateConduitNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UPixel2DTDAnimStateConduitNode::GetOutputPin() const
{
	return Pins[1];
}

void UPixel2DTDAnimStateConduitNode::GetTransitionList(TArray<class UPixel2DTDAnimStateTransitionNode*>& OutTransitions, bool bWantSortedList)
{
	// Normal transitions
	for (int32 LinkIndex = 0; LinkIndex < Pins[1]->LinkedTo.Num(); ++LinkIndex)
	{
		UEdGraphNode* TargetNode = Pins[1]->LinkedTo[LinkIndex]->GetOwningNode();
		if (UPixel2DTDAnimStateTransitionNode* Transition = Cast<UPixel2DTDAnimStateTransitionNode>(TargetNode))
		{
			OutTransitions.Add(Transition);
		}
	}

	// Sort the transitions by priority order, lower numbers are higher priority
	if (bWantSortedList)
	{
		struct FCompareTransitionsByPriority
		{
			FORCEINLINE bool operator()(const UPixel2DTDAnimStateTransitionNode& A, const UPixel2DTDAnimStateTransitionNode& B) const
			{
				return A.PriorityOrder < B.PriorityOrder;
			}
		};

		OutTransitions.Sort(FCompareTransitionsByPriority());
	}
}

void UPixel2DTDAnimStateConduitNode::PostPlacedNewNode()
{
	// Create a new animation graph
	check(BoundGraph == NULL);
	BoundGraph = FBlueprintEditorUtils::CreateNewGraph(
		this,
		NAME_None,
		UPixel2DTDAnimTransitionGraph::StaticClass(),
		UPixel2DTDAnimStateConduitGraphSchema::StaticClass());
	check(BoundGraph);

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, TEXT("Conduit"));

	// Initialize the transition graph
	const UEdGraphSchema* Schema = BoundGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*BoundGraph);

	// Add the new graph as a child of our parent graph
	UEdGraph* ParentGraph = GetGraph();

	if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
	{
		ParentGraph->SubGraphs.Add(BoundGraph);
	}
}

void UPixel2DTDAnimStateConduitNode::DestroyNode()
{
	UEdGraph* GraphToRemove = BoundGraph;

	BoundGraph = NULL;
	Super::DestroyNode();

	if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FPixel2DAnimBlueprintUtils::RemoveGraph(Blueprint, GraphToRemove, EGraphRemoveFlags::Recompile);
	}
}

void UPixel2DTDAnimStateConduitNode::ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const
{
	Super::ValidateNodeDuringCompilation(MessageLog);

	UPixel2DTDAnimTransitionGraph* TransGraph = CastChecked<UPixel2DTDAnimTransitionGraph>(BoundGraph);
	UPixel2DTDAnimGraphNode_TransitionResult* ResultNode = TransGraph->GetResultNode();
	check(ResultNode);

	UEdGraphPin* BoolResultPin = ResultNode->Pins[0];
	if ((BoolResultPin->LinkedTo.Num() == 0) && (BoolResultPin->DefaultValue.ToBool() == false))
	{
		MessageLog.Warning(TEXT("@@ will never be taken, please connect something to @@"), this, BoolResultPin);
	}
}

FString UPixel2DTDAnimStateConduitNode::GetDesiredNewNodeName() const
{
	return TEXT("Conduit");
}

void UPixel2DTDAnimStateConduitNode::PostPasteNode()
{
	// Find an interesting name, but try to keep the same if possible
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, GetStateName());
	Super::PostPasteNode();
}

#undef LOCTEXT_NAMESPACE
