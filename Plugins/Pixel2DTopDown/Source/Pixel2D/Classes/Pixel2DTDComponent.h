//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Pixel2DTDLayerManager.h"
#include "PaperFlipbookComponent.h"
#include "Pixel2DTDFlipbookComponent.h"
#include "Pixel2DTDComponent.generated.h"

class UPixel2DTDAnimInstance;

USTRUCT()
struct PIXEL2DTD_API FPixel2DTDLayerMember
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Pixel2DTopDown)
	FName LayerName;

	UPROPERTY(EditAnywhere, Category=Pixel2DTopDown)
	uint8 LayerDepthWithin;

};

UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = Paper2D, meta = (BlueprintSpawnableComponent))
class PIXEL2DTD_API UPixel2DTDComponent : public UPixel2DTDFlipbookComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class TSubclassOf<UPixel2DTDAnimInstance> SpriteAnimInstance;

	/** The active animation graph program instance. */
	UPROPERTY(transient, NonTransactional)
	UPixel2DTDAnimInstance* AnimScriptInstance;

	/** Used to scale speed of all animations on this skeletal mesh. */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Animation)
	float GlobalAnimRateScale;

	UPROPERTY(Category = Sprite, EditAnywhere)
	FPixel2DTDLayerMember LayerMember;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

public:
	void SetAnimInstance(UClass * NewAnimInstance);

	// Management function for if we want to do an evaluation but may already be running one
	// bBlockOnTask - if true and we are currently performing parallel eval we wait for it to finish
	// bPerformPostAnimEvaluation - if true and we are currently performing parallel eval we call PostAnimEvaluation too
	// return true if parallel task was running.
	bool HandleExistingParallelEvaluationTask(bool bBlockOnTask, bool bPerformPostAnimEvaluation);

private:
	void InitAnim();
	bool InitializeAnimScriptInstance();

	/** @return whether we should tick animation (we may want to skip it due to URO) */
	bool ShouldTickAnimation() const;

	/** Tick Animation system */
	void TickAnimation(float DeltaTime, bool bNeedsValidRootMotion);

	//~ Begin UActorComponent Interface.
protected:
	virtual void OnRegister() override;
public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.

	/** Evaluate Anim System **/
	UPaperFlipbook* EvaluateAnimation(const UPixel2DTDComponent* InSkeletalMesh, UPixel2DTDAnimInstance* InAnimInstance, UPaperFlipbook * OutPose, bool& bLooping) const;
	bool NeedToSpawnAnimScriptInstance() const;

};
