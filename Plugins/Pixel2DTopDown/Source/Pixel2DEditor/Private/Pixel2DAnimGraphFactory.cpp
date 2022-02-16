//Polyart Studio 2021


#include "Pixel2DAnimGraphFactory.h"

#include "Pixel2DAnimGraph/Pixel2DTDAnimGraphNode_Root.h"
#include "Pixel2DAnimGraph/Pixel2DTDAnimGraphNode_StateMachineBase.h"

#include "Pixel2DTDAnimStateTransitionNode.h"
#include "Pixel2DTDAnimStateNode.h"
#include "Pixel2DTDAnimStateConduitNode.h"
#include "Pixel2DTDAnimStateMachineSchema.h"
#include "Pixel2DTDAnimGraphSchema.h"

#include "SPixel2DGraphNodeTransition.h"
#include "SPixel2DGraphNodeConduit.h"
#include "SPixel2DGraphNodeStateEntry.h"

#include "SPixel2DGraphNodeResult.h"
#include "SPixel2DGraphNodeNode.h"
#include "SPixel2DGraphNodeStateMachineInstance.h"

#include "Pixel2DAnimGraph/Pixel2DStateMachineConnectionDrawingPolicy.h"
#include "Pixel2DAnimGraph/Pixel2DAnimGraphConnectionDrawingPolicy.h"

#include "KismetPins/SGraphPinExec.h"


TSharedPtr<class SGraphNode> FPixel2DAnimGraphNodeFactory::CreateNode(class UEdGraphNode* InNode) const
{
	if (UPixel2DTDAnimGraphNode_Base* BaseAnimNode = Cast<UPixel2DTDAnimGraphNode_Base>(InNode))
	{
		if (UPixel2DTDAnimGraphNode_Root* RootAnimNode = Cast<UPixel2DTDAnimGraphNode_Root>(InNode))
		{
			return SNew(SPixel2DGraphNodeResult, RootAnimNode);
		}
		else if (UPixel2DTDAnimGraphNode_StateMachineBase* StateMachineInstance = Cast<UPixel2DTDAnimGraphNode_StateMachineBase>(InNode))
		{
			return SNew(SPixel2DGraphNodeStateMachineInstance, StateMachineInstance);
		}
		else
		{
			return SNew(SPixel2DGraphNodeNode, BaseAnimNode);
		}
	}
	else if (UPixel2DTDAnimStateTransitionNode* TransitionNode = Cast<UPixel2DTDAnimStateTransitionNode>(InNode))
	{
		return SNew(SPixel2DGraphNodeTransition, TransitionNode);
	}
	else if (UPixel2DTDAnimStateNode* StateNode = Cast<UPixel2DTDAnimStateNode>(InNode))
	{
		return SNew(SPixel2DGraphNodeState, StateNode);
	}
	else if (UPixel2DTDAnimStateConduitNode* ConduitNode = Cast<UPixel2DTDAnimStateConduitNode>(InNode))
	{
		return SNew(SPixel2DGraphNodeConduit, ConduitNode);
	}
	else if (UPixel2DTDAnimStateEntryNode* EntryNode = Cast<UPixel2DTDAnimStateEntryNode>(InNode))
	{
		return SNew(SPixel2DGraphNodeStateEntry, EntryNode);
	}

	return nullptr;
}

TSharedPtr<class SGraphPin> FPixel2DAnimGraphPinFactory::CreatePin(class UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if (InPin->PinType.PinSubCategoryObject == FPixel2DTDPoseLink::StaticStruct())
		{
			return SNew(SGraphPinExec, InPin);
		}
	}

	if (InPin->PinType.PinCategory == UPixel2DTDAnimStateMachineSchema::PC_Exec)
	{
		return SNew(SGraphPinExec, InPin);
	}

	return nullptr;
}


class FConnectionDrawingPolicy* FPixel2DAnimGraphPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UPixel2DTDAnimGraphSchema::StaticClass()))
	{
		return new FPixel2DAnimGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	else if (Schema->IsA(UPixel2DTDAnimStateMachineSchema::StaticClass()))
	{
		return new FPixel2DStateMachineConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;
}