//Polyart Studio 2021


#include "Pixel2DTDAnimBlueprintFactory.h"
#include "Pixel2DTDAnimBlueprint.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EdGraphSchema_K2.h"

#include "Pixel2DTDAnimInstance.h"
#include "Pixel2DTDAnimGraph.h"
#include "Pixel2DTDAnimGraphSchema.h"

#include "AnimationGraph.h"
#include "AnimationGraphSchema.h"

UPixel2DTDAnimBlueprintFactory::UPixel2DTDAnimBlueprintFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UPixel2DTDAnimBlueprint::StaticClass();

	bCreateNew = true;
	bEditorImport = true;
	bEditAfterNew = true;
	ParentClass = UPixel2DTDAnimInstance::StaticClass();
}

UObject* UPixel2DTDAnimBlueprintFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, NAME_None);
}

UObject* UPixel2DTDAnimBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UPixel2DTDAnimBlueprint::StaticClass()));
	UPixel2DTDAnimBlueprint * SpriteAnimBP = CastChecked<UPixel2DTDAnimBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType, UPixel2DTDAnimBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));

	// Add the pixel anim graph
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(SpriteAnimBP, K2Schema->GN_AnimGraph, UPixel2DTDAnimGraph::StaticClass(), UPixel2DTDAnimGraphSchema::StaticClass());
	FBlueprintEditorUtils::AddDomainSpecificGraph(SpriteAnimBP, NewGraph);
	SpriteAnimBP->LastEditedDocuments.Add(NewGraph);
	NewGraph->bAllowDeletion = false;

	// Mark the BP as being regenerated, so it will not be confused as needing to be loaded and regenerated when a referenced BP loads.
	SpriteAnimBP->bHasBeenRegenerated = true;

	return SpriteAnimBP;
}