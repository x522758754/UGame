// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/GNpcPathFollowingComponent.h"

void UGNpcPathFollowingComponent::OnSegmentFinished()
{
	Super::OnSegmentFinished();
	
	FunctionSegmentFinished.ExecuteIfBound(GetCurrentPathIndex());
}

void UGNpcPathFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGNpcPathFollowingComponent::OnPathFinished(const FPathFollowingResult& Result)
{
	Super::OnPathFinished(Result);

	FunctionPathFinished.ExecuteIfBound(Result);
}

bool UGNpcPathFollowingComponent::HasReachedCurrentTarget(const FVector& CurrentLocation) const
{
	if (MovementComp == NULL)
	{
		return false;
	}

	const FVector CurrentTarget = GetCurrentTargetLocation();
	const FVector CurrentDirection = GetCurrentDirection();

	// check if moved too far
	const FVector ToTarget = (CurrentTarget - MovementComp->GetActorFeetLocation());
	const float SegmentDot = FVector::DotProduct(ToTarget, CurrentDirection);
	if (SegmentDot < 0.0)
	{
		return true;
	}

	// or standing at target position
	// don't use acceptance radius here, it has to be exact for moving near corners (2D test < 5% of agent radius)
	float GoalRadius = 0.0f;
	float GoalHalfHeight = 0.0f;
	if(bIsPrevReachedSegmentLocation)
	{
		GoalRadius = MovementComp->GetMaxSpeed() * 0.1f;
		GoalHalfHeight = 100.0f;
	}


	return HasReachedInternal(CurrentTarget, GoalRadius, GoalHalfHeight, CurrentLocation, CurrentAcceptanceRadius, 0.05f);
}
