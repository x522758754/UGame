//Polyart Studio 2021


#include "Pixel2DEditorPawnSensingModule.h"
#include "Modules/ModuleManager.h"

#include "CoreMinimal.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"

#include "Pixel2DTDPawnSensingComponent.h"
#include "Pixel2DSensingComponentVisualizer.h"
#include "UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"


DECLARE_LOG_CATEGORY_EXTERN(LogEditorPawnSensing, Log, All);

DEFINE_LOG_CATEGORY(LogEditorPawnSensing)

#define LOCTEXT_NAMESPACE "Pixel2DEditorPawnSensing"


//////////////////////////////////////////////////////////////////////////
// FPixel2DEditorPawnSensingModule

class FPixel2DEditorPawnSensingModule : public IPixel2DEditorPawnSensingModule
{
public:
	FPixel2DEditorPawnSensingModule()
	{}

public:
	virtual void StartupModule() override
	{
		// Register custom component visualizations
		{

			if (GUnrealEd)
			{
				GUnrealEd->RegisterComponentVisualizer(UPixel2DTDPawnSensingComponent::StaticClass()->GetFName(), MakeShareable(new FPixel2DSensingComponentVisualizer));
			}
		}

	}

	virtual void ShutdownModule() override
	{
		if (GUnrealEd)
		{
			GUnrealEd->UnregisterComponentVisualizer(UPixel2DTDPawnSensingComponent::StaticClass()->GetFName());
		}
	}   
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FPixel2DEditorPawnSensingModule, Pixel2DEditorPawnSensingTD);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
