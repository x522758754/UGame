// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GNpcAIController.generated.h"

class UGAIItem;
/**
 * 
 */
UCLASS()
class RUNNER_API AGNpcAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGNpcAIController(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY()
	UGAIItem* NpcAIItem;
};
