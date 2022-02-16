//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"

#include "Pixel2DTDAnimBlueprint.generated.h"

class UBlueprint;
class FCompilerResultsLog;
struct FKismetCompilerOptions;
class FKismetCompilerContext;

UCLASS(BlueprintType, meta = (DisplayThumbnail = "true"))
class  PIXEL2DTD_API UPixel2DTDAnimBlueprint : public UBlueprint
{
	GENERATED_UCLASS_BODY()

	// REVISIT: to be removed
	UPROPERTY(EditAnywhere, Category = Optimization)
	bool bUseMultiThreadedAnimationUpdate;

	virtual void Serialize(FArchive& Ar) override;

#if WITH_EDITOR

	virtual UClass* GetBlueprintClass() const override;

	//// UBlueprint interface
	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}

	virtual bool IsValidForBytecodeOnlyRecompile() const override { return false; }
	virtual bool CanRecompileWhilePlayingInEditor() const override;

	virtual void PostLoad() override;
#endif	// #if WITH_EDITOR

};


