//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FPixel2DAnimNotifyCommands : public TCommands<FPixel2DAnimNotifyCommands>
{
public:
	FPixel2DAnimNotifyCommands()
		: TCommands<FPixel2DAnimNotifyCommands>(
			TEXT("Pixel2DAnimNotifyEditor"), // Context name for fast lookup
			NSLOCTEXT("Contexts", "Pixel2DEditorTD", "Pixel2D TopDown AnimNotify Editor"), // Localized context name for displaying
			NAME_None, // Parent
			FName(TEXT("Pixel2DStyle")) // Icon Style Set
			)
	{
	}

	// TCommand<> interface
	virtual void RegisterCommands() override;
	// End of TCommand<> interface

public:
	TSharedPtr<FUICommandInfo> UpdateNotificationPosition;

};
