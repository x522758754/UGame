// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Npc/GNpcInfoComponent.h"

#include "Level/GLevelFunctions.h"

void UGNpcInfoComponent::SetConfigId(int32 InConfigId)
{
	NpcConfig.ConfigId = InConfigId;
	ConfigId = InConfigId;
}

void UGNpcInfoComponent::InitNpcConfig(const FGNpcConfig& InNpcConfig)
{
	ConfigId = InNpcConfig.ConfigId;
	NpcConfig = InNpcConfig;

	ApplyNpcConfig();
}

void UGNpcInfoComponent::ApplyNpcConfig()
{
}

void UGNpcInfoComponent::AcquireNpcConfig()
{
	if (!ConfigId)
	{
		return;
	}

	auto* Value = UGLevelFunctions::GetNpcConfig(ConfigId);
	if (!Value)
	{
		if (GetOwner())
		{
			UE_LOG(LogTemp, Error, TEXT("UGNpcInfoComponent::AcquireConfig Actor=(%s),Id=%d"),
				*GetOwner()->GetName(), ConfigId);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UGNpcInfoComponent::AcquireConfig Id=%d"), ConfigId);
		}

		return;
	}
	NpcConfig = *Value;
	ApplyNpcConfig();
}

void UGNpcInfoComponent::RegisterToSubsystem()
{
}

void UGNpcInfoComponent::OnRep_ConfigId()
{
	Super::OnRep_ConfigId();
	
}
