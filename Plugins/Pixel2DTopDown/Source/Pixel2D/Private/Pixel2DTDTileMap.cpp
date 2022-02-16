//Polyart Studio 2021


#include "Pixel2DTDTileMap.h"
#include "PaperFlipbookComponent.h"
#include "Pixel2DTDTileMapComponent.h"

#define LOCTEXT_NAMESPACE "Pixel2DTopDown"

//////////////////////////////////////////////////////////////////////////
// UPixel2DTDTileMap

UPixel2DTDTileMap::UPixel2DTDTileMap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPixel2DTDTileMap::PreSave(const class ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);

#if WITH_EDITOR
	for (TObjectIterator<UPixel2DTDTileMapComponent> TileMapIt; TileMapIt; ++TileMapIt)
	{
		if (UPixel2DTDTileMapComponent* TestComponent = *TileMapIt)
		{
			if (TestComponent->TileMap == this)
			{
				TestComponent->SpawnFlipbooks();
			}
		}
	}
#endif
}

void UPixel2DTDTileMap::AddFlipbook(UPaperFlipbook* Flipbook, UPaperTileLayer* Layer, FIntPoint Location)
{
	FPixelTDPaintedFlipbook NewEntry{ Flipbook, Layer, Location };

	FPixelTDPaintedFlipbook* FoundEntry = Flipbooks.FindByPredicate([&NewEntry](FPixelTDPaintedFlipbook Entry)
		{ return (Entry.Layer == NewEntry.Layer) && (Entry.Location == NewEntry.Location); });

	if (FoundEntry != nullptr)
	{
		FoundEntry->Source = NewEntry.Source;
	}
	else
	{
		Flipbooks.Add(NewEntry);
	}
}

void UPixel2DTDTileMap::RemoveFlipbook(UPaperTileLayer* Layer, FIntPoint Location)
{
	Flipbooks.RemoveAll([&](FPixelTDPaintedFlipbook& Entry)
		{ return ((Entry.Layer == Layer) || (Layer == nullptr)) && (Entry.Location == Location); });
}

UPaperFlipbook* UPixel2DTDTileMap::GetFlipbook(UPaperTileLayer* Layer, FIntPoint Location)
{
	FPixelTDPaintedFlipbook* FoundEntry = Flipbooks.FindByPredicate([&](FPixelTDPaintedFlipbook& Entry)
		{ return ((Entry.Layer == Layer) || (Layer == nullptr)) && (Entry.Location == Location); });

	if (FoundEntry != nullptr)
	{
		return FoundEntry->Source;
	}

	return nullptr;
}

void UPixel2DTDTileMap::ClearAllFlipbooks()
{
	Flipbooks.Reset();
}

void UPixel2DTDTileMap::DeleteLayer(int32 DeleteIndex)
{
	check(DeleteIndex <= TileLayers.Num());

	UPaperTileLayer* DeletedLayer = TileLayers[DeleteIndex];
	Flipbooks.RemoveAllSwap([&](FPixelTDPaintedFlipbook& Entry)
		{
			return (Entry.Layer == DeletedLayer);
		});

	TileLayers.RemoveAt(DeleteIndex);
}

UPaperTileLayer* UPixel2DTDTileMap::DuplicateLayer(int32 DuplicateIndex)
{
	check(DuplicateIndex <= TileLayers.Num());

	UPaperTileLayer* SourceLayer = TileLayers[DuplicateIndex];
	UPaperTileLayer* NewLayer = DuplicateObject<UPaperTileLayer>(SourceLayer, this);
	TileLayers.Insert(NewLayer, DuplicateIndex);

	const int32 PreNumFlipbooks = Flipbooks.Num();
	for (int32 Index = 0; Index < PreNumFlipbooks; Index++)
	{
		if (Flipbooks[Index].Layer == SourceLayer)
		{
			Flipbooks.Add({ Flipbooks[Index].Source, NewLayer, Flipbooks[Index].Location });
		}
	}

	return NewLayer;
}

#if WITH_EDITOR

void UPixel2DTDTileMap::PruneOutOfRangeFlipbooks()
{
	Flipbooks.RemoveAllSwap([this](FPixelTDPaintedFlipbook& Entry)
		{
			return (this->MapHeight <= Entry.Location.Y) || (this->MapWidth <= Entry.Location.X);
		});
}

void UPixel2DTDTileMap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	PruneOutOfRangeFlipbooks();

	for (TObjectIterator<UPixel2DTDTileMapComponent> TileMapIt; TileMapIt; ++TileMapIt)
	{
		if (UPixel2DTDTileMapComponent* TestComponent = *TileMapIt)
		{
			if (TestComponent->TileMap == this)
			{
				TestComponent->RefreshPreview();
			}
		}
	}

}
#endif

#undef LOCTEXT_NAMESPACE