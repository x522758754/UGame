//Polyart Studio 2021


#pragma once
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Pixel2DStyle.h"

class FPixel2DLayerCommands : public TCommands<FPixel2DLayerCommands>
{
public:

	FPixel2DLayerCommands()
		: TCommands<FPixel2DLayerCommands>(TEXT("Slat"), NSLOCTEXT("Contexts", "Slat", "Slat Plugin"), NAME_None, FPixel2DStyle::GetStyleSetName())//NAME_None)//TEXT("None"))
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};