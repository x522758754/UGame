//Polyart Studio 2021


/*=============================================================================
Pixel2DTDAnimStateNodeBase.cpp
=============================================================================*/

#include "Pixel2DTDAnimStateNodeBase.h"
#include "UObject/FrameworkObjectVersion.h"
#include "Pixel2DTDAnimBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimStateMachineGraph.h"
#include "Pixel2DTDAnimStateMachineSchema.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/KismetEditorUtilities.h"

/////////////////////////////////////////////////////
// FPixel2DAnimStateNodeNameValidator

class FPixel2DAnimStateNodeNameValidator : public FStringSetNameValidator
{
public:
	FPixel2DAnimStateNodeNameValidator(const UPixel2DTDAnimStateNodeBase* InStateNode)
		: FStringSetNameValidator(FString())
	{
		TArray<UPixel2DTDAnimStateNodeBase*> Nodes;
		UPixel2DTDAnimStateMachineGraph* StateMachine = CastChecked<UPixel2DTDAnimStateMachineGraph>(InStateNode->GetOuter());

		StateMachine->GetNodesOfClass<UPixel2DTDAnimStateNodeBase>(Nodes);
		for (auto NodeIt = Nodes.CreateIterator(); NodeIt; ++NodeIt)
		{
			auto Node = *NodeIt;
			if (Node != InStateNode)
			{
				Names.Add(Node->GetStateName());
			}
		}
	}
};

/////////////////////////////////////////////////////
// UPixel2DTDAnimStateNodeBase

UPixel2DTDAnimStateNodeBase::UPixel2DTDAnimStateNodeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimStateNodeBase::PostPasteNode()
{
	Super::PostPasteNode();

	if (UEdGraph* BoundGraph = GetBoundGraph())
	{
		// Add the new graph as a child of our parent graph
		UEdGraph* ParentGraph = GetGraph();

		if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
		{
			ParentGraph->SubGraphs.Add(BoundGraph);
		}

		// Restore transactional flag that is lost during copy/paste process
		BoundGraph->SetFlags(RF_Transactional);

		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(ParentGraph);
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	}
}

UObject* UPixel2DTDAnimStateNodeBase::GetJumpTargetForDoubleClick() const
{
	return GetBoundGraph();
}

bool UPixel2DTDAnimStateNodeBase::CanJumpToDefinition() const
{
	return GetJumpTargetForDoubleClick() != nullptr;
}

void UPixel2DTDAnimStateNodeBase::JumpToDefinition() const
{
	if (UObject* HyperlinkTarget = GetJumpTargetForDoubleClick())
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(HyperlinkTarget);
	}
}

bool UPixel2DTDAnimStateNodeBase::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return Schema->IsA(UPixel2DTDAnimStateMachineSchema::StaticClass());
}

void UPixel2DTDAnimStateNodeBase::OnRenameNode(const FString& NewName)
{
	FBlueprintEditorUtils::RenameGraph(GetBoundGraph(), NewName);
}

TSharedPtr<class INameValidatorInterface> UPixel2DTDAnimStateNodeBase::MakeNameValidator() const
{
	return MakeShareable(new FPixel2DAnimStateNodeNameValidator(this));
}

void UPixel2DTDAnimStateNodeBase::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FFrameworkObjectVersion::GUID);
}

void UPixel2DTDAnimStateNodeBase::PostLoad()
{
	Super::PostLoad();

	const int32 CustomVersion = GetLinkerCustomVersion(FFrameworkObjectVersion::GUID);

	if (CustomVersion < FFrameworkObjectVersion::FixNonTransactionalPins)
	{
		int32 BrokenPinCount = 0;
		for (UEdGraphPin_Deprecated* Pin : DeprecatedPins)
		{
			if (!Pin->HasAnyFlags(RF_Transactional))
			{
				++BrokenPinCount;
				Pin->SetFlags(Pin->GetFlags() | RF_Transactional);
			}
		}

		if (BrokenPinCount > 0)
		{
			UE_LOG(LogAnimation, Log, TEXT("Fixed %d non-transactional pins in %s"), BrokenPinCount, *GetName());
		}
	}
}

UPixel2DTDAnimBlueprint* UPixel2DTDAnimStateNodeBase::GetSpriteAnimBlueprint() const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNode(this);
	return CastChecked<UPixel2DTDAnimBlueprint>(Blueprint);
}