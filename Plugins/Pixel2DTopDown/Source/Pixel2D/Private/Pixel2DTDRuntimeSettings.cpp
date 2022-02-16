//Polyart Studio 2021


#include "Pixel2DTDRuntimeSettings.h"

//////////////////////////////////////////////////////////////////////////
// UPixel2DTDRuntimeSettings

UPixel2DTDRuntimeSettings::UPixel2DTDRuntimeSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
#if WITH_EDITORONLY_DATA
	, TileReplacerDataTable("/Game/Pixel2D/Blueprints/AnimatedTileReplacer/Pixel2DTileReplacerDataTable.Pixel2DTDTileReplacerDataTable")
#endif
{
}