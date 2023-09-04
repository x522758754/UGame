// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "GNpcPathFollowingComponent.generated.h"

DECLARE_DELEGATE_OneParam(FPathFollowDelegate, const FPathFollowingResult&);
DECLARE_DELEGATE_OneParam(FPathFollowSegementDelegate, int32);

/**
 *
 */
UCLASS()
class UGNpcPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

	friend class AGNpcAIController;
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnSegmentFinished() override;
	virtual void OnPathFinished(const FPathFollowingResult& Result) override;
	virtual bool HasReachedCurrentTarget(const FVector& CurrentLocation) const override;
protected:
	FPathFollowSegementDelegate FunctionSegmentFinished;
	FPathFollowDelegate FunctionPathFollowResume;
	FPathFollowDelegate FunctionPathFinished;
protected:
	bool bIsPrevReachedSegmentLocation = false;	//提前到达寻路点
};