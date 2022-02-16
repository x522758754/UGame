//Polyart Studio 2021


#include "Pixel2DEditorTileReplacerModule.h"
#include "Modules/ModuleManager.h"

#include "CoreMinimal.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"

#include "Pixel2DTDTileReplacer.h"


DECLARE_LOG_CATEGORY_EXTERN(LogEditorTileReplacer, Log, All);

DEFINE_LOG_CATEGORY(LogEditorTileReplacer)

#define LOCTEXT_NAMESPACE "Pixel2DTDEditorTileReplacerModule"


//////////////////////////////////////////////////////////////////////////
// FPixel2DEditorPawnSensingModule

class FPixel2DEditorTileReplacerModule : public IPixel2DEditorTileReplacerModule
{
public:
	FPixel2DEditorTileReplacerModule()
	{}

private:
	TSharedPtr<FPixel2DTDTileReplacer> TileReplacer;

public:
	virtual void StartupModule() override
	{
		// Tile Replacer 
		{
			TileReplacer = MakeShareable(new FPixel2DTDTileReplacer());
			TileReplacer->Initialize();
		}

	}

	virtual void ShutdownModule() override
	{
		if (TileReplacer.IsValid())
		{
			TileReplacer->Uninitialize();
		}
	}   
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FPixel2DEditorTileReplacerModule, Pixel2DEditorTileReplacerTD);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
