//Polyart Studio 2021


#include "Pixel2DTDAnimGraphNode_Base.h"
#include "Pixel2DTDAnimNode_Base.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDAnimGraphSchema.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "Pixel2DAnimBlueprintNodeOptionalPinManager.h"
#include "IAnimNodeEditMode.h"
#include "AnimNodeEditModes.h"
#include "Pixel2DTDAnimGraph.h"
#include "EditorModeManager.h"
#include "Toolkits/AssetEditorManager.h"

/////////////////////////////////////////////////////
// UPixel2DTDAnimGraphNode_Base

UPixel2DTDAnimGraphNode_Base::UPixel2DTDAnimGraphNode_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDAnimGraphNode_Base::PreEditChange(FProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);

	if (PropertyThatWillChange && PropertyThatWillChange->GetFName() == GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin))
	{
		FOptionalPinManager::CacheShownPins(ShowPinForProperties, OldShownPins);
	}
}

void UPixel2DTDAnimGraphNode_Base::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if ((PropertyName == GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin)))
	{
		FOptionalPinManager::EvaluateOldShownPins(ShowPinForProperties, OldShownPins, this);
		GetSchema()->ReconstructNode(*this);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);

	PropertyChangeEvent.Broadcast(PropertyChangedEvent);
}

void UPixel2DTDAnimGraphNode_Base::CreateOutputPins()
{
	if (!IsSinkNode())
	{
		const UPixel2DTDAnimGraphSchema* Schema = GetDefault<UPixel2DTDAnimGraphSchema>();
		CreatePin(EGPD_Output, Schema->PC_Struct, FPixel2DTDPoseLink::StaticStruct(), TEXT("Sprite Pose"));
	}
}

void UPixel2DTDAnimGraphNode_Base::InternalPinCreation(TArray<UEdGraphPin*>* OldPins)
{
	// preload required assets first before creating pins
	PreloadRequiredAssets();

	const UPixel2DTDAnimGraphSchema* Schema = GetDefault<UPixel2DTDAnimGraphSchema>();
	if (const FStructProperty* NodeStruct = GetFNodeProperty())
	{
		// Display any currently visible optional pins
		{
			UObject* NodeDefaults = GetArchetype();
			FPixel2DAnimBlueprintNodeOptionalPinManager OptionalPinManager(this, OldPins);
			OptionalPinManager.AllocateDefaultPins(NodeStruct->Struct, NodeStruct->ContainerPtrToValuePtr<uint8>(this), NodeDefaults ? NodeStruct->ContainerPtrToValuePtr<uint8>(NodeDefaults) : nullptr);
		}

		// Create the output pin, if needed
		CreateOutputPins();
	}
}

void UPixel2DTDAnimGraphNode_Base::AllocateDefaultPins()
{
	InternalPinCreation(NULL);
}

void UPixel2DTDAnimGraphNode_Base::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	InternalPinCreation(&OldPins);

	RestoreSplitPins(OldPins);
}

bool UPixel2DTDAnimGraphNode_Base::CanJumpToDefinition() const
{
	return GetJumpTargetForDoubleClick() != nullptr;
}

void UPixel2DTDAnimGraphNode_Base::JumpToDefinition() const
{
	if (UObject* HyperlinkTarget = GetJumpTargetForDoubleClick())
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(HyperlinkTarget);
	}
}

FLinearColor UPixel2DTDAnimGraphNode_Base::GetNodeTitleColor() const
{
	return FLinearColor::Black;
}

UScriptStruct* UPixel2DTDAnimGraphNode_Base::GetFNodeType() const
{
	UScriptStruct* BaseFStruct = FPixel2DTDAnimNode_Base::StaticStruct();

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct->IsChildOf(BaseFStruct))
			{
				return StructProp->Struct;
			}
		}
	}

	return NULL;
}

FStructProperty* UPixel2DTDAnimGraphNode_Base::GetFNodeProperty() const
{
	UScriptStruct* BaseFStruct = FPixel2DTDAnimNode_Base::StaticStruct();

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct->IsChildOf(BaseFStruct))
			{
				return StructProp;
			}
		}
	}

	return NULL;
}

FString UPixel2DTDAnimGraphNode_Base::GetNodeCategory() const
{
	return TEXT("Misc.");
}

void UPixel2DTDAnimGraphNode_Base::GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const
{
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Type"), TEXT("SpriteAnimGraphNode")));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Class"), GetClass()->GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Name"), GetName()));
}

void UPixel2DTDAnimGraphNode_Base::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UPixel2DTDAnimGraphNode_Base::GetMenuCategory() const
{
	return FText::FromString(GetNodeCategory());
}

void UPixel2DTDAnimGraphNode_Base::GetPinAssociatedProperty(const UScriptStruct* NodeType, const UEdGraphPin* InputPin, FProperty*& OutProperty, int32& OutIndex) const
{
	OutProperty = nullptr;
	OutIndex = INDEX_NONE;

	//@TODO: Name-based hackery, avoid the roundtrip and better indicate when it's an array pose pin
	const FString PinNameStr = InputPin->PinName.ToString();
	int32 UnderscoreIndex = PinNameStr.Find(TEXT("_"), ESearchCase::CaseSensitive);
	if (UnderscoreIndex != INDEX_NONE)
	{
		FString ArrayName = PinNameStr.Left(UnderscoreIndex);
		int32 ArrayIndex = FCString::Atoi(*(PinNameStr.Mid(UnderscoreIndex + 1)));

		if (FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(NodeType, *ArrayName))
		{
			OutProperty = ArrayProperty;
			OutIndex = ArrayIndex;
		}
	}

	// If the array check failed or we have no underscores
	if (OutProperty == nullptr)
	{
		if (FProperty* Property = FindFProperty<FProperty>(NodeType, *(PinNameStr)))
		{
			OutProperty = Property;
			OutIndex = INDEX_NONE;
		}
	}
}

FPixel2DTDPoseLinkMappingRecord UPixel2DTDAnimGraphNode_Base::GetLinkIDLocation(const UScriptStruct* NodeType, UEdGraphPin* SourcePin)
{
	if (SourcePin->LinkedTo.Num() > 0)
	{
		if (UPixel2DTDAnimGraphNode_Base* LinkedNode = Cast<UPixel2DTDAnimGraphNode_Base>(FBlueprintEditorUtils::FindFirstCompilerRelevantNode(SourcePin->LinkedTo[0])))
		{
			//@TODO: Name-based hackery, avoid the roundtrip and better indicate when it's an array pose pin
			const FString SourcePinName = SourcePin->PinName.ToString();
			const int32 UnderscoreIndex = SourcePinName.Find(TEXT("_"), ESearchCase::CaseSensitive);
			if (UnderscoreIndex != INDEX_NONE)
			{
				const FString ArrayName = SourcePinName.Left(UnderscoreIndex);

				if (FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(NodeType, *ArrayName))
				{
					if (FStructProperty* Property = CastField<FStructProperty>(ArrayProperty->Inner))
					{
						if (Property->Struct->IsChildOf(FPixel2DTDPoseLinkBase::StaticStruct()))
						{
							const int32 ArrayIndex = FCString::Atoi(*(SourcePinName.Mid(UnderscoreIndex + 1)));
							return FPixel2DTDPoseLinkMappingRecord::MakeFromArrayEntry(this, LinkedNode, ArrayProperty, ArrayIndex);
						}
					}
				}
			}
			else
			{
				if (FStructProperty* Property = FindFProperty<FStructProperty>(NodeType, SourcePin->PinName))
				{
					if (Property->Struct->IsChildOf(FPixel2DTDPoseLinkBase::StaticStruct()))
					{
						return FPixel2DTDPoseLinkMappingRecord::MakeFromMember(this, LinkedNode, Property);
					}
				}
			}
		}
	}

	return FPixel2DTDPoseLinkMappingRecord::MakeInvalid();
}


void UPixel2DTDAnimGraphNode_Base::PostProcessPinName(const UEdGraphPin* Pin, FString& DisplayName) const
{
	if (Pin->Direction == EGPD_Output)
	{
		if (Pin->PinName == TEXT("Sprite Pose"))
		{
			DisplayName.Reset();
		}
	}
}

bool UPixel2DTDAnimGraphNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UPixel2DTDAnimGraphSchema::StaticClass());
}

void UPixel2DTDAnimGraphNode_Base::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	if (UPixel2DTDAnimGraphSchema::IsLocalSpacePosePin(Pin.PinType))
	{
		HoverTextOut = TEXT("Pixel2D Top-Down Animation Pose");
	}
	else
	{
		Super::GetPinHoverText(Pin, HoverTextOut);
	}
}

FEditorModeID UPixel2DTDAnimGraphNode_Base::GetEditorMode() const
{
	return AnimNodeEditModes::AnimNode;
}

EAnimAssetHandlerType UPixel2DTDAnimGraphNode_Base::SupportsAssetClass(const UClass* AssetClass) const
{
	return EAnimAssetHandlerType::NotSupported;
}

void UPixel2DTDAnimGraphNode_Base::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	CopyPinDefaultsToNodeData(Pin);

	if (UPixel2DTDAnimGraph* AnimationGraph = Cast<UPixel2DTDAnimGraph>(GetGraph()))
	{
		AnimationGraph->OnPinDefaultValueChanged.Broadcast(Pin);
	}
}

bool UPixel2DTDAnimGraphNode_Base::IsPinExposedAndLinked(const FString& InPinName, const EEdGraphPinDirection InDirection) const
{
	UEdGraphPin* Pin = FindPin(InPinName, InDirection);
	return Pin != nullptr && Pin->LinkedTo.Num() > 0 && Pin->LinkedTo[0] != nullptr;
}