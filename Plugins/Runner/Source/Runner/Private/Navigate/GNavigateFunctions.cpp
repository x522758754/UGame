// Fill out your copyright notice in the Description page of Project Settings.


#include "Navigate//GNavigateFunctions.h"

#include "NavigationSystem.h"
#include "Player/GPlayerController.h"
#include "System/GCommonFunctions.h"


UGPathFollowingComponent* UGNavigateFunctions::GetPathFollowingComponent()
{
	auto PC = UGCommonFunctions::GetPlayerController();
	if(PC == nullptr)
	{
		return nullptr;
	}

	UGPathFollowingComponent* PathFollowingComponent = PC->FindComponentByClass<UGPathFollowingComponent>();
	check(PathFollowingComponent)

	return PathFollowingComponent;
}

FAIRequestID UGNavigateFunctions::NavigateToGoal(const FVector& GoalLocation, const TFunction<void(FAIRequestID, const FPathFollowingResult& Result)>& OnRequestFinished)
{
	uint32 RequestID = FAIRequestID::InvalidRequest;
	auto PC = UGCommonFunctions::GetPlayerController();
	UNavigationSystemV1* NavSys = PC ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(PC->GetWorld()) : nullptr;
	if (NavSys == nullptr || PC == nullptr || PC->GetPawn() == nullptr)
	{
		UE_LOG(LogNavigation, Warning, TEXT("UGNavigateFunctions::NavigateToGoal called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(NavSys), *GetNameSafe(PC), PC ? *GetNameSafe(PC->GetPawn()) : TEXT("NULL"));
		return RequestID;
	}

	const FNavAgentProperties& AgentProps = PC->GetNavAgentPropertiesRef();
	const FVector AgentNavLocation = PC->GetNavAgentLocation();

	const ANavigationData* NavData = NavSys->GetNavDataForProps(AgentProps, AgentNavLocation);
	if (!NavData)
	{
		return RequestID;
	}
	
	FNavLocation ProjectedLocation;
	if (!NavSys->ProjectPointToNavigation(GoalLocation, ProjectedLocation, INVALID_NAVEXTENT, &AgentProps))
	{
		return RequestID;
	}
	

	UGPathFollowingComponent* PathFollowingComp = GetPathFollowingComponent();
	
	//PathFollowingComp->OnRequestFinished.Clear();
	//PathFollowingComp->OnRequestFinished.AddLambda(OnRequestFinished);
	if (!PathFollowingComp->IsPathFollowingAllowed())
	{
		return RequestID;
	}

	const bool bAlreadyAtGoal = PathFollowingComp->HasReached(GoalLocation, EPathFollowingReachMode::OverlapAgent);
	if (bAlreadyAtGoal)
	{
		RequestID = PathFollowingComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	}
	else
	{
		// script source, keep only one move request at time
		if (PathFollowingComp->GetStatus() != EPathFollowingStatus::Idle)
		{
			PathFollowingComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::AnyRequest, EPathFollowingVelocityMode::Keep);
		}
		
		FPathFindingQuery Query(PC, *NavData, AgentNavLocation, GoalLocation);
		FPathFindingResult Result = NavSys->FindPathSync(Query);
		if (Result.IsSuccessful())
		{
			RequestID = PathFollowingComp->RequestMove(FAIMoveRequest(GoalLocation), Result.Path);
		}
		else if (PathFollowingComp->GetStatus() != EPathFollowingStatus::Idle)
		{
			RequestID = PathFollowingComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
		}
	}

	return RequestID;
}

void UGNavigateFunctions::AbortMove()
{
	GetPathFollowingComponent()->AbortMove(*UGCommonFunctions::GetPlayerController(), FPathFollowingResultFlags::UserAbortFlagMask);
}


bool UGNavigateFunctions::IsNavigating()
{
	UGPathFollowingComponent* PathFollowingComp = GetPathFollowingComponent();
	if(PathFollowingComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		return true;
	}
	return false;
}

bool UGNavigateFunctions::IsNavigatingToGoalLocation(const FVector& GoalLocation)
{
	if(IsNavigating())
	{
		FVector PathDestination = GetPathFollowingComponent()->GetPathDestination();

		return FVector::Dist(GoalLocation, PathDestination) < GetPathFollowingComponent()->GetAcceptanceRadius();
	}
	return false;
}

bool UGNavigateFunctions::IsNavigatingToGoal(AActor* Goal)
{
	return Goal == GetPathFollowingComponent()->GetMoveGoal();
}

FPathFindingResult UGNavigateFunctions::GetPathFindingResult(const FVector& StartLocation, const FVector& GoalLocation, bool IsUsingPartialPaths)
{
	FPathFindingResult Result(ENavigationQueryResult::Error);

	if(GetPathFollowingComponent()->IsPathFollowingAllowed())
	{
		auto PC = UGCommonFunctions::GetPlayerController();
		if (UNavigationSystemV1* NavSys = PC ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(PC->GetWorld()) : nullptr)
		{
			if(const ANavigationData* NavData = NavSys->GetNavDataForProps(PC->GetPawn()->GetNavAgentPropertiesRef(), StartLocation))
			{
				FPathFindingResult PathResult;
				FPathFindingQuery PFQuery(*PC->GetPawn(), *NavData, StartLocation, GoalLocation);
				PFQuery.SetAllowPartialPaths(IsUsingPartialPaths);
				Result = NavSys->FindPathSync(PFQuery);
			}
		}
	}

	return Result;
}

bool UGNavigateFunctions::CanNavigateToLocation(const FVector& GoalLocation)
{
	const FVector StartLocation = UGCommonFunctions::GetPlayerController()->GetNavAgentLocation();
	FPathFindingResult Result = GetPathFindingResult(StartLocation, GoalLocation);
	if (Result.IsSuccessful() && Result.Path.IsValid())
	{
		//是否可以一次醒寻路
		if(!Result.IsPartial())
		{
			return true;
		}
	}

	return false;
}

bool UGNavigateFunctions::GetNavigateToGoalLocation(TArray<FNavPathPoint>& PathPoints, const FVector& GoalLocation)
{
	PathPoints.Empty();

	FVector StartLocation = UGCommonFunctions::GetPlayerController()->GetNavAgentLocation();
	bool bContinue = false;
	do
	{
		FPathFindingResult Result = GetPathFindingResult(StartLocation, GoalLocation);
		bContinue = Result.IsSuccessful() && Result.Path.IsValid() && Result.IsPartial();
		if(Result.IsSuccessful() && Result.Path.IsValid())
		{
			PathPoints.Append(Result.Path->GetPathPoints());
			StartLocation = Result.Path->GetEndLocation();

			float Radius = GetPathFollowingComponent()->GetAcceptanceRadius();
			if(Radius > FVector::Dist(Result.Path->GetEndLocation(), Result.Path->GetStartLocation()))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	while (bContinue);
	
	return true;
}
