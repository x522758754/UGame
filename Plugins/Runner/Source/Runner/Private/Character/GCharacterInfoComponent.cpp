// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GCharacterInfoComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values for this component's properties
UGCharacterInfoComponent::UGCharacterInfoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGCharacterInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	RegisterToSubsystem();
}

void UGCharacterInfoComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnregisterFromSubsystem();
}

void UGCharacterInfoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams RepLifetimeParams;
	RepLifetimeParams.bIsPushBased = true;
	RepLifetimeParams.Condition = COND_InitialOnly;
	DOREPLIFETIME_WITH_PARAMS(UGCharacterInfoComponent, ConfigId, RepLifetimeParams);
}


// Called every frame
void UGCharacterInfoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGCharacterInfoComponent::SetConfigId(int32 InConfigId)
{
	ConfigId = InConfigId;
}

void UGCharacterInfoComponent::SetGuid(int64 InGuid)
{
	Guid = InGuid;
	RegisterToSubsystem();
}

int64 UGCharacterInfoComponent::GetGuid()
{
	return Guid;
}

int32 UGCharacterInfoComponent::GetConfigId()
{
	return ConfigId;
}

void UGCharacterInfoComponent::RegisterToSubsystem()
{
}

void UGCharacterInfoComponent::UnregisterFromSubsystem()
{
}

void UGCharacterInfoComponent::OnRep_ConfigId()
{
	 
}

void UGCharacterInfoComponent::OnRep_Guid()
{
	RegisterToSubsystem();
}

