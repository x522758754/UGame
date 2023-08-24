// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GLevelFunctions.generated.h"

class AGHeroCharacter;
struct FGNpcConfig;
/**
 *
 */
UCLASS()
class  UGLevelFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static int32 GetLevelId(const FString& MapName);
	
	static bool FitLocation(UPARAM(Ref)FVector& Location, float CapsuleHalfHeight, UWorld* World = nullptr, float ExtendLength = 0);

	static const FGNpcConfig* UGLevelFunctions::GetNpcConfig(int NpcConfigId);
};



