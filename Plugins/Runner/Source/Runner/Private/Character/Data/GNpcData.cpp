// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Data/GNpcData.h"

#include "Character/Npc/GNpcFunctions.h"
#include "Character/Npc/GNpcInfoComponent.h"

void FGClientNpcInfo::Spawn(UWorld* World)
{
	NpcInfoComponent = UGNpcFunctions::Client_SpawnNpc(NpcConfig, World);
	if(!NpcInfoComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("FGClientNpcInfo::Spawn Client_SpawnNpc Error, %d"), NpcConfig.ConfigId);
		return;
	}
	NpcActor = NpcInfoComponent->GetOwner();
	if(!bFirstSpawn)
	{
		NpcActor->SetActorLocation(Location);
	}
	if(AfterNpcSpawnFunction)
	{
		AfterNpcSpawnFunction(*this);
	}
	bFirstSpawn = false;
}

