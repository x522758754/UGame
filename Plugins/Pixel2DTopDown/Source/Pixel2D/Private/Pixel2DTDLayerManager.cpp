//Polyart Studio 2021


#include "Pixel2DTDLayerManager.h"

#define LOCTEXT_NAMESPACE "FPixel2DLayerManager"

TArray<TSharedPtr<FPixel2DTDLayer>> UPixel2DTDLayerManager::Layers;
bool UPixel2DTDLayerManager::isVisible = true;

UPixel2DTDLayerManager::UPixel2DTDLayerManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDLayerManager::AddLayer()
{
	FText NewName = FText::Format(LOCTEXT("layermanager","newlayername{0}"), FindDefaultDuplicates());
	uint8 NewLayerID = FindAvailableID();

	TSharedPtr<FPixel2DTDLayer> NewLayer(new FPixel2DTDLayer(NewName,NewLayerID));
	UPixel2DTDLayerManager::Layers.Add(NewLayer);
}

void UPixel2DTDLayerManager::DeleteLayer(TSharedPtr<FPixel2DTDLayer> DeletedLayer)
{
	uint8 DeletedLayerID = DeletedLayer->LayerID;
	Layers.RemoveSingle(DeletedLayer);

	for (auto Layer : Layers)
		if (Layer->LayerID > DeletedLayerID)
			Layer->LayerID--;
}

void UPixel2DTDLayerManager::ChangeDepth(TSharedPtr<FPixel2DTDLayer> Layer, bool Increment)
{
	if ((Increment && (Layer->LayerID == 0)) ||
		(!Increment && (Layer->LayerID == Layers.Num()-1)))
		return;

	uint8 OldLayerID = Layer->LayerID;
	uint8 NewLayerID = (Increment ? Layer->LayerID-1 : Layer->LayerID+1);
	int32 OldIndex = -1;
	int32 NewIndex = -1;


	for (int i = 0; i < Layers.Num(); i++)
	{
		if (Layers[i]->LayerID == NewLayerID)
		{
			Layers[i]->LayerID = OldLayerID;
			OldIndex = i;
		}
		if (Layers[i] == Layer)
			NewIndex = i;
	}

	Layer->LayerID = NewLayerID;

	Layers.Swap(OldIndex, NewIndex);
}

int UPixel2DTDLayerManager::GetNumLayers()
{
	return UPixel2DTDLayerManager::Layers.Num();
}

TArray<TSharedPtr<FPixel2DTDLayer>> UPixel2DTDLayerManager::GetLayers()
{
	return Layers;
}

bool UPixel2DTDLayerManager::GetIsVisible(TSharedPtr<FPixel2DTDLayer> LayerName)
{
	return isVisible;
}

void UPixel2DTDLayerManager::SetIsVisible(bool bVisible)
{
	isVisible = bVisible;
}

bool UPixel2DTDLayerManager::GetIsLocked(TSharedPtr<FPixel2DTDLayer> LayerName)
{
	return false;
}

uint16 UPixel2DTDLayerManager::FindDefaultDuplicates()
{
	return Layers.Num()+1;
}

uint8 UPixel2DTDLayerManager::FindAvailableID()
{
	bool UsedIDs[255];
	
	for (int i = 0; i < 255; i++)
		UsedIDs[i] = false;

	for (auto Layer : Layers)
	{
		UsedIDs[Layer->LayerID] = true;
	}

	for (uint8 i = 0; i < 255; i++)
		if (UsedIDs[i] == false)
			return i;

	return 0;
}

void UPixel2DTDLayerManager::GetLayersByName(TArray<TSharedPtr<FString>> &Container)
{
	for (auto Layer : Layers)
	{
		TSharedPtr<FString> LayerName(new FString((Layer->Name).ToString()));
		Container.Add(LayerName);
	}
}

#undef LOCTEXT_NAMESPACE
