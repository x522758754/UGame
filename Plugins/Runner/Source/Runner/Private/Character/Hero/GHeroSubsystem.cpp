// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroSubsystem.h"

#include "Character/Hero/GHeroCharacter.h"
#include "Character/Hero/GHeroInfoComponent.h"

#include "Event/GEventBasic.h"

#include "System/GGameInstance.h"

TWeakObjectPtr<UGHeroSubsystem> UGHeroSubsystem::s_Instance = nullptr;
UGHeroSubsystem* UGHeroSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGHeroSubsystem>();
	}
	return s_Instance.Get();
}

void UGHeroSubsystem::AddInfoComponent(UGHeroInfoComponent* Component)
{
	if (!Component)
	{
		return;
	}
	uint64 Guid = Component->GetGuid();
	if (Guid == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGPawnInfoSubsystem::AddPawnInfoComponent %s Guid=0"),
			*Component->GetName());
		return;
	}

	if (InfoMap.Contains(Guid))
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("UGPawnInfoSubsystem::AddPawnInfoComponent %s Guid=%lld"), *Component->GetName(), Guid);

	InfoMap.Add(Guid, Component);
	UGEventBasicFunctions::Dispatch(EGEventType::PlayerInfoAdded, Component);
}

void UGHeroSubsystem::RemoveInfoComponent(UGHeroInfoComponent* Component)
{
	int64 Guid = Component->GetGuid();
	if (!InfoMap.Contains(Guid))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGPawnInfoSubsystem::RemovePawnInfoComponent %s Guid=%lld"), *Component->GetName(), Guid);
		return;
	}
	UGEventBasicFunctions::Dispatch(EGEventType::PlayerInfoRemoved, Component);
	InfoMap.Remove(Guid);
	UE_LOG(LogTemp, Log, TEXT("UGPawnInfoSubsystem::RemovePawnInfoComponent %s Guid=%lld"), *Component->GetName(), Guid);
}

UGHeroInfoComponent* UGHeroSubsystem::GetInfoComponent(int64 Guid)
{
	return InfoMap.Contains(Guid) ? InfoMap[Guid] : nullptr;
}

AGHeroCharacter* UGHeroSubsystem::GetHeroCharacter(int64 Guid)
{
	return GetInfoComponent(Guid) ? Cast<AGHeroCharacter>(GetInfoComponent(Guid)->GetOwner()) : nullptr;
}

bool UGHeroSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UGHeroSubsystem::OnGameInstanceInit()
{
	Super::OnGameInstanceInit();
}

void UGHeroSubsystem::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);
}