// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAIItem.h"
#include "Components/SplineComponent.h"

#include "GAIPatrol.generated.h"

USTRUCT(BlueprintType)
struct FGNpcPatrolParam
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="AI")
	FSplineCurves SplineCurves;

	UPROPERTY(VisibleAnywhere, Category="AI")
	FRotator SplineRelationRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float WaitSeconds;

	UPROPERTY(VisibleAnywhere)
	bool IsValid = false;
};

UCLASS(BlueprintType)
class UGAIPatrol : public UGAIItem
{
	GENERATED_BODY()

public:
	virtual void RefreshConfig(AActor* NpcActor);

	USplineComponent* MakeSplineComponent(AActor* NpcActor);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGNpcPatrolParam PatrolParam;
};