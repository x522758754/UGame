//Polyart Studio 2021


#include "Pixel2DLayerCommands.h"

#define LOCTEXT_NAMESPACE "FPixel2DLayerModule"

void FPixel2DLayerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Pixel2DLayers", "Bring up Pixel2D TopDown Layers window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
