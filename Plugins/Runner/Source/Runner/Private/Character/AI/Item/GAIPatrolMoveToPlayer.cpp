// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Item/GAIPatrolMoveToPlayer.h"

#include "Actor/Router/GRouter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GNpcAIController.h"

void UGAIPatrolMoveToPlayer::RefreshConfig(AActor* NpcActor)
{
#if WITH_EDITOR
	Super::RefreshConfig(NpcActor);
	if(NpcActor)
	{
		if(USplineComponent *SplineComponent = NpcActor->FindComponentByClass<USplineComponent>())
		{
			PatrolParam.SplineCurves = SplineComponent->SplineCurves;
			PatrolParam.Transform = SplineComponent->GetComponentTransform();
			PatrolParam.IsValid = true;
		}
	}
#endif
}

bool UGAIPatrolMoveToPlayer::IgnoreBehaviorTree()
{
	return PatrolParam.IsValid;
}

void UGAIPatrolMoveToPlayer::Init(AActor* NpcActor)
{
	MakeSplineComponent(NpcActor);

	if(IgnoreBehaviorTree())
	{
		UseCustomMove();
	}
}

void UGAIPatrolMoveToPlayer::SetNpcAIController(AGNpcAIController* Controller)
{
	Super::SetNpcAIController(Controller);
}

void UGAIPatrolMoveToPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(WaitSeconds > 0.0f)
	{
		WaitSeconds -= DeltaSeconds;
		if(WaitSeconds <= 0.0f)
		{
			PatrolToTarget();
		}
	}
}

USplineComponent* UGAIPatrolMoveToPlayer::MakeSplineComponent(AActor* NpcActor)
{
	if(!NpcActor)
	{
		return nullptr;
	}
	
	if(!PatrolParam.IsValid)
	{
		return nullptr;
	}

	AGRouter* RouterActor = GetWorld()->SpawnActor<AGRouter>(AGRouter::StaticClass(), PatrolParam.Transform);
	if(!RouterActor)
	{
		return nullptr;
	}
	
	RouterActor->GetSplineComponent()->SplineCurves = PatrolParam.SplineCurves;
	RouterActor->GetSplineComponent()->UpdateSpline();

	SplineComponentPtr = RouterActor->GetSplineComponent();
	
	return RouterActor->GetSplineComponent();
}

void UGAIPatrolMoveToPlayer::UseCustomMove()
{
	if(!SplineComponentPtr.IsValid() || !NpcAIController.IsValid())
	{
		return;
	}
	NpcAIController->GetPathFinished().BindUObject(this, &UGAIPatrolMoveToPlayer::OnPathFollowArrived);

	PatrolToTarget();
}

void UGAIPatrolMoveToPlayer::OnSegmentFinished(int CurrentPathIndex)
{
}

void UGAIPatrolMoveToPlayer::OnPathFollowArrived(const FPathFollowingResult& Result)
{
	if(!SplineComponentPtr.IsValid() || !NpcAIController.IsValid())
	{
		return;
	}
	float TempTargetSplineDistance = TargetSplineDistance + PatrolParam.DistanceIncreaseGap;
	TargetSplineDistance = UKismetMathLibrary::SelectFloat(TempTargetSplineDistance, 0.0, TempTargetSplineDistance < SplineComponentPtr->GetSplineLength());

	if(PatrolParam.WaitSeconds > 0)
	{
		WaitSeconds = PatrolParam.WaitSeconds;
	}
	else
	{
		PatrolToTarget();
	}
}

void UGAIPatrolMoveToPlayer::OnResumePathFollow()
{
}

void UGAIPatrolMoveToPlayer::PatrolToTarget()
{
	TargetPoint = SplineComponentPtr->GetLocationAtDistanceAlongSpline(TargetSplineDistance, ESplineCoordinateSpace::World);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(NpcAIController.Get(), TargetPoint);
}
