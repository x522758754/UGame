// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/GAIItem.h"

#include "Config/GConfigDataAsset.h"

#include "Player/GNpcAIController.h"

void UGAIItem::RefreshConfig(AActor* NpcActor)
{
	auto Value = UGConfigDataAsset::Get()->AIConfigs.Find(GetClass());
	if(Value)
	{
		this->BehaviorTree = *Value;
	}
}

void UGAIItem::SetNpcAIController(AGNpcAIController* Controller)
{
	NpcAIController = Controller;
}

AGNpcAIController* UGAIItem::GetNpcAIController() const
{
	return NpcAIController;
}