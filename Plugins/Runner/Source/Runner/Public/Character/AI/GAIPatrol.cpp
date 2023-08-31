// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/GAIPatrol.h"

#include "Player/GNpcAIController.h"

void UGAIPatrol::RefreshConfig(AActor* NpcActor)
{
	Super::RefreshConfig(NpcActor);

	if(USplineComponent* SplineComponent = NpcActor->FindComponentByClass<USplineComponent>())
	{
		PatrolParam.SplineCurves = SplineComponent->SplineCurves;
		PatrolParam.SplineRelationRotation = SplineComponent->GetRelativeRotation();
		PatrolParam.IsValid = true;
	}
	else
	{
		PatrolParam.IsValid = false;
	}
}

USplineComponent* UGAIPatrol::MakeSplineComponent(AActor* NpcActor)
{
	if(!NpcActor)
	{
		return false;
	}
	
	if(!PatrolParam.IsValid)
	{
		return false;
	}
	
	USplineComponent* SplineComponent = NewObject<USplineComponent>(NpcActor, "SplineComponent");
	SplineComponent->AttachToComponent(NpcActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	SplineComponent->RegisterComponent();
	SplineComponent->SetMobility(EComponentMobility::Static);
	NpcActor->AddInstanceComponent(SplineComponent);

	SplineComponent->SetRelativeRotation(PatrolParam.SplineRelationRotation);
	SplineComponent->SplineCurves = PatrolParam.SplineCurves;
	SplineComponent->UpdateSpline();

	return SplineComponent;
}
