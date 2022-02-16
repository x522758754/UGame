//Polyart Studio 2021


#include "Pixel2DTDTileMapComponent.h"
#include "Pixel2DTDTileMap.h"
#include "PaperSpriteComponent.h"
#include "Pixel2DTDFlipbookComponent.h"
#include "Pixel2DTDTileMapActor.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Public/ActorGroupingUtils.h"
#endif

#include "Pixel2DTDTileMapRenderSceneProxy.h"

#define LOCTEXT_NAMESPACE "Pixel2DTopDown"

//////////////////////////////////////////////////////////////////////////
// UPixel2DTDTileMapComponent

UPixel2DTDTileMapComponent::UPixel2DTDTileMapComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPixel2DTDTileMapComponent::~UPixel2DTDTileMapComponent()
{
#if WITH_EDITOR
	DestroyPreview();
#endif

}

void UPixel2DTDTileMapComponent::CreateNewOwnedTileMap()
{
	TGuardValue<TEnumAsByte<EComponentMobility::Type>> MobilitySaver(Mobility, EComponentMobility::Movable);

	UPaperTileMap* NewTileMap = NewObject<UPixel2DTDTileMap>(this);
	NewTileMap->SetFlags(RF_Transactional);
	NewTileMap->InitializeNewEmptyTileMap();

	SetTileMap(NewTileMap);
	ClearSpawnedFlipbooks();
}

void UPixel2DTDTileMapComponent::CreateNewTileMap(int32 MapWidth, int32 MapHeight, int32 TileWidth, int32 TileHeight, float PixelsPerUnrealUnit, bool bCreateLayer)
{
	TGuardValue<TEnumAsByte<EComponentMobility::Type>> MobilitySaver(Mobility, EComponentMobility::Movable);

	UPaperTileMap* NewTileMap = NewObject<UPixel2DTDTileMap>(this);
	NewTileMap->SetFlags(RF_Transactional);
	NewTileMap->MapWidth = MapWidth;
	NewTileMap->MapHeight = MapHeight;
	NewTileMap->TileWidth = TileWidth;
	NewTileMap->TileHeight = TileHeight;
	NewTileMap->PixelsPerUnrealUnit = PixelsPerUnrealUnit;

	if (bCreateLayer)
	{
		NewTileMap->AddNewLayer();
	}

	SetTileMap(NewTileMap);
	ClearSpawnedFlipbooks();
}

void UPixel2DTDTileMapComponent::PostInitProperties()
{
	EObjectFlags Flags = HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject) ?
		GetMaskedFlags(RF_PropagateToSubObjects) : RF_NoFlags;
	// In a post GFastPathUniqueNameGeneration world we have to provide a stable name
	// for all archetypes, here I'm using PaperTileMap_0 to match old content:
	TileMap = NewObject<UPixel2DTDTileMap>(this, TEXT("Pixel2DTileMap_0"), Flags);

	TileMap->SetFlags(RF_Transactional);

	UMeshComponent::PostInitProperties();
}

void UPixel2DTDTileMapComponent::ClearSpawnedFlipbooks()
{
	for (UPixel2DTDFlipbookComponent* Entry : SpawnedFlipbooks)
	{
		if (Entry != nullptr)
		{
			Entry->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			Entry->UnregisterComponent();
			Entry->DestroyComponent();
		}
	}

	SpawnedFlipbooks.Empty();
}

void UPixel2DTDTileMapComponent::SpawnFlipbooks()
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	APixel2DTDTileMapActor* ParentActor = Cast<APixel2DTDTileMapActor>(this->GetOwner());

	if (!ParentActor || ParentActor->IsPendingKill())
	{
		return;
	}

	ClearSpawnedFlipbooks();

	PixelTileMap->ForEachFlipbook([&](FPixelTDPaintedFlipbook& PaintedFlipbook)
		{
			if (!PaintedFlipbook.Layer->ShouldRenderInGame())
			{
				return;
			}

			UPixel2DTDFlipbookComponent* SpawnedComponent = NewObject<UPixel2DTDFlipbookComponent>(this->GetOwner());
			SpawnedComponent->SetFlipbook(PaintedFlipbook.Source);

			int TileX = PaintedFlipbook.Location.X;
			int TileY = PaintedFlipbook.Location.Y;
			int LayerIndex = PaintedFlipbook.Layer->GetLayerIndex();
			FVector LocalPosition = PixelTileMap->GetTileCenterInLocalSpace(TileX, TileY, LayerIndex);

			SpawnedComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			SpawnedComponent->RegisterComponent();
			SpawnedComponent->SetRelativeLocation(LocalPosition);

			SpawnedFlipbooks.Add(SpawnedComponent);
		});
}

void UPixel2DTDTileMapComponent::AddFlipbook(UPaperFlipbook* Flipbook, UPaperTileLayer* Layer, FIntPoint Location)
{
	if ((Flipbook == nullptr) || (Layer == nullptr) || (Location == FIntPoint::NoneValue))
	{
		return;
	}

	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	PixelTileMap->AddFlipbook(Flipbook, Layer, Location);
}

void UPixel2DTDTileMapComponent::RemoveFlipbook(UPaperTileLayer* Layer, FIntPoint Location)
{
#if WITH_EDITOR
	const FPixelTDPaintedFlipbook Entry{ nullptr, Layer, Location };

	UPixel2DTDFlipbookComponent** ExistingPreview = PreviewComponentMap.Find(Entry.GetId());

	if (ExistingPreview)
	{
		if (*ExistingPreview)
		{
			(*ExistingPreview)->SetFlipbook(nullptr);
		}
	}
#endif

	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	PixelTileMap->RemoveFlipbook(Layer, Location);
}

UPaperFlipbook* UPixel2DTDTileMapComponent::GetFlipbook(UPaperTileLayer* Layer, FIntPoint Location)
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	return PixelTileMap->GetFlipbook(Layer, Location);
}

void UPixel2DTDTileMapComponent::ClearAllFlipbooks()
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	PixelTileMap->ClearAllFlipbooks();

#if WITH_EDITOR
	PreviewComponentMap.Empty();

	for (size_t Index = 0; Index < PreviewComponents.Num(); Index++)
	{
		if (PreviewComponents[Index] != nullptr)
		{
			PreviewComponents[Index]->SetFlipbook(nullptr);
		}
	}
#endif
}

#if WITH_EDITOR
bool UPixel2DTDTileMapComponent::CanPreview()
{
	AActor* ParentActor = Cast<AActor>(this->GetOwner());
	return (ParentActor == nullptr);
}

void UPixel2DTDTileMapComponent::RebuildPreview()
{

	if (!CanPreview())
	{
		return;
	}

	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	int NumNeededComponents = PixelTileMap->NumFlipbooks() - PreviewComponents.Num();

	for (int Index = 0; Index < NumNeededComponents; Index++)
	{
		UPixel2DTDFlipbookComponent* Component = NewObject<UPixel2DTDFlipbookComponent>();
		Component->UpdateBounds();
		Component->AddToRoot();
		Component->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		Component->RegisterComponentWithWorld(PreviewWorld);

		PreviewComponents.Add(Component);
	}

	LoadPreviewComponents();
}

void UPixel2DTDTileMapComponent::LoadPreviewComponents()
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();
	int32 Index = 0;
	int32 ExtraComponentsNum = (PreviewComponents.Num() - PixelTileMap->NumFlipbooks());

	ensure(ExtraComponentsNum >=0);

	PreviewComponentMap.Empty();

	PixelTileMap->ForEachFlipbook([&](FPixelTDPaintedFlipbook& PaintedFlipbook)
		{
			if (PreviewComponents[Index] == nullptr)
				return;

			PreviewComponents[Index]->SetFlipbook(PaintedFlipbook.Source);
			PreviewComponentMap.Add(PaintedFlipbook.GetId(), PreviewComponents[Index]);

			int TileX = PaintedFlipbook.Location.X;
			int TileY = PaintedFlipbook.Location.Y;
			int LayerIndex = PaintedFlipbook.Layer->GetLayerIndex();
			FVector LocalPosition = PixelTileMap->GetTileCenterInLocalSpace(TileX, TileY, LayerIndex);

			PreviewComponents[Index]->SetRelativeLocation(LocalPosition);

			Index++;
		});

	for (Index = 0; Index < ExtraComponentsNum; Index++)
	{
		int32 EmptyIndex = Index + PixelTileMap->NumFlipbooks();
		if (PreviewComponents[EmptyIndex] == nullptr)
			continue;
		PreviewComponents[EmptyIndex]->SetFlipbook(nullptr);
	}
}

void UPixel2DTDTileMapComponent::RefreshPreviewVisibility()
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();

	PixelTileMap->ForEachFlipbook([&](FPixelTDPaintedFlipbook& PaintedFlipbook)
		{
			UPaperTileLayer* TargetLayer = PaintedFlipbook.Layer;
			int32 FlipbookIndex = PaintedFlipbook.GetId();

			UPixel2DTDFlipbookComponent** Preview = PreviewComponentMap.Find(PaintedFlipbook.GetId());
			if (Preview && *Preview)
			{
				if ((*Preview)->IsVisible() != TargetLayer->ShouldRenderInEditor())
				{
					(*Preview)->SetVisibility(!(*Preview)->IsVisible());
				}
			}
		});
}

void UPixel2DTDTileMapComponent::RefreshPreview()
{
	UPixel2DTDTileMap* PixelTileMap = GetTileMap();

	// REVISIT - highly inefficient can be optimised to update just what has changed
	RebuildPreview();
	RefreshPreviewVisibility();
}

void UPixel2DTDTileMapComponent::DestroyPreview()
{
	for (size_t Index = 0; Index < PreviewComponents.Num(); Index++)
	{
		if (PreviewComponents[Index] != nullptr)
		{
			PreviewComponents[Index]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			PreviewComponents[Index]->RemoveFromRoot();
			PreviewComponents[Index]->UnregisterComponent();
			PreviewComponents[Index] = nullptr;
		}
	}
}

void UPixel2DTDTileMapComponent::RegisterComponentWithWorld(UWorld* InWorld, FRegisterComponentContext* Context)
{
	Super::RegisterComponentWithWorld(InWorld, Context);
	PreviewWorld = InWorld;
}

#endif

#undef LOCTEXT_NAMESPACE