// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/GPathFollowingComponent.h"

UGPathFollowingComponent::UGPathFollowingComponent()
{
}

void UGPathFollowingComponent::OnPathFinished(const FPathFollowingResult& Result)
{
	Super::OnPathFinished(Result);
}
