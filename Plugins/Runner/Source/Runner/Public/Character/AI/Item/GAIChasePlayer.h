// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BlackboardData.h"

#include "Character/AI/GAIItem.h"
#include "Components/SplineComponent.h"

#include "GAIChasePlayer.generated.h"

struct FPathFollowingResult;


USTRUCT(BlueprintType)
struct FGNpcMoveToPatrolParam
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="AI")
	FSplineCurves SplineCurves;

	UPROPERTY(VisibleAnywhere, Category="AI")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float WaitSeconds;

	UPROPERTY(VisibleAnywhere)
	bool IsValid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="每次沿着曲线组件增加的距离"))
	float DistanceIncreaseGap = 50.f;
};

UCLASS(BlueprintType)
class UGAIChasePlayer : public UGAIItem
{
	GENERATED_BODY()

public:
	virtual void RefreshConfig(AActor* NpcActor) override;
protected:
	virtual void Init(AActor* NpcActor) override;
	virtual void SetNpcAIController(AGNpcAIController* Controller) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnSegmentFinished(int CurrentPathIndex);
	virtual void OnPathFollowArrived(const FPathFollowingResult& Result);
	virtual void OnResumePathFollow();

	void PatrolToTarget();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightDistance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolRadius = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseRadius = 200.f;

protected:
	FVector BornLocation;
};
