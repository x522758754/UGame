// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "GPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

	friend class UGNavigateFunctions;
public:
	UGPathFollowingComponent();

protected:
	virtual void OnPathFinished(const FPathFollowingResult& Result) override;
};
