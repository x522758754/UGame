// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AI/GAIItem.h"
#include "Components/SplineComponent.h"

#include "GAIPatrolAlongSpline.generated.h"

struct FPathFollowingResult;


USTRUCT(BlueprintType)
struct FGPatrolSplineParam
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
class UGAIPatrolAlongSpline : public UGAIItem
{
	GENERATED_BODY()

public:
	virtual void RefreshConfig(AActor* NpcActor) override;
	virtual bool IgnoreBehaviorTree() override;
protected:
	virtual void Init(AActor* NpcActor) override;
	virtual void SetNpcAIController(AGNpcAIController* Controller) override;
	virtual void Tick(float DeltaSeconds) override;
	
	USplineComponent* MakeSplineComponent(AActor* NpcActor);
	void UseCustomMove();

	virtual void OnSegmentFinished(int CurrentPathIndex);
	virtual void OnPathFollowArrived(const FPathFollowingResult& Result);
	virtual void OnResumePathFollow();

	void PatrolToTarget();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGPatrolSplineParam PatrolParam;

	TWeakObjectPtr<USplineComponent> SplineComponentPtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ToolTip="沿着曲线组件增加的总计目标距离"))
	float TargetSplineDistance = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ToolTip="当前目标点"))
	FVector TargetPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ToolTip="当前等待时间"))
	float WaitSeconds;
};
