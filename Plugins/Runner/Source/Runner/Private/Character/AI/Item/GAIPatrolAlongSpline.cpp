// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Item/GAIPatrolAlongSpline.h"

#include "Actor/Router/GRouter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Level/GLevelSubsystem.h"

#include "Player/GNpcAIController.h"

void UGAIPatrolAlongSpline::RefreshConfig(AActor* NpcActor)
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

bool UGAIPatrolAlongSpline::IgnoreBehaviorTree()
{
	return PatrolParam.IsValid;
}

void UGAIPatrolAlongSpline::Init(AActor* NpcActor)
{
	MakeSplineComponent(NpcActor);

	if(IgnoreBehaviorTree())
	{
		UseCustomMove();
	}
}

void UGAIPatrolAlongSpline::SetNpcAIController(AGNpcAIController* Controller)
{
	Super::SetNpcAIController(Controller);
}

void UGAIPatrolAlongSpline::Tick(float DeltaSeconds)
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

USplineComponent* UGAIPatrolAlongSpline::MakeSplineComponent(AActor* NpcActor)
{
	if(!NpcActor)
	{
		return nullptr;
	}
	
	if(!PatrolParam.IsValid)
	{
		return nullptr;
	}

	AActor* RouterActor = UGLevelSubsystem::Get()->GetRouterActor();
	if(!RouterActor)
	{
		return nullptr;
	}
	
	USplineComponent* SplineComponent = NewObject<USplineComponent>(RouterActor, "SplineComponent");
	SplineComponent->SetWorldTransform(PatrolParam.Transform);
	SplineComponent->AttachToComponent(RouterActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	SplineComponent->RegisterComponent();
	RouterActor->AddInstanceComponent(SplineComponent);
	SplineComponent->SetMobility(EComponentMobility::Static);
	
	SplineComponent->SplineCurves = PatrolParam.SplineCurves;
	SplineComponent->UpdateSpline();

	SplineComponentPtr = SplineComponent;
	
	return SplineComponent;
}

void UGAIPatrolAlongSpline::UseCustomMove()
{
	if(!SplineComponentPtr.IsValid() || !NpcAIController.IsValid())
	{
		return;
	}
	NpcAIController->GetPathFinished().BindUObject(this, &UGAIPatrolAlongSpline::OnPathFollowArrived);

	PatrolToTarget();
}

void UGAIPatrolAlongSpline::OnSegmentFinished(int CurrentPathIndex)
{
}

void UGAIPatrolAlongSpline::OnPathFollowArrived(const FPathFollowingResult& Result)
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

void UGAIPatrolAlongSpline::OnResumePathFollow()
{
}

void UGAIPatrolAlongSpline::PatrolToTarget()
{
	TargetPoint = SplineComponentPtr->GetLocationAtDistanceAlongSpline(TargetSplineDistance, ESplineCoordinateSpace::World);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(NpcAIController.Get(), TargetPoint);
}