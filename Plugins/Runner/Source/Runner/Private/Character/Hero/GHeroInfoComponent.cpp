// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroInfoComponent.h"

#include "Character/Hero/GHeroSubsystem.h"

UGHeroInfoComponent::UGHeroInfoComponent()
{
}

void UGHeroInfoComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGHeroInfoComponent::RegisterToSubsystem()
{
	if(Guid && GetOwner())
	{
		UGHeroSubsystem::Get()->AddInfoComponent(this);
	}
}

void UGHeroInfoComponent::UnregisterFromSubsystem()
{
	if(Guid)
	{
		UGHeroSubsystem::Get()->RemoveInfoComponent(this);
	}
}
