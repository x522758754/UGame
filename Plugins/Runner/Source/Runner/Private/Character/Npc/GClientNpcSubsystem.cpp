// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Npc/GClientNpcSubsystem.h"

#include "Camera/GPlayerCameraManager.h"
#include "System/GCommonFunctions.h"
#include "Config/GGameConfigSettings.h"
#include "System/GGameInstance.h"
#include "Event/GEventObserverHolder.h"

TWeakObjectPtr<UGClientNpcSubsystem> UGClientNpcSubsystem::s_Instance = nullptr;
UGClientNpcSubsystem* UGClientNpcSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGClientNpcSubsystem>();
	}
	return s_Instance.Get();
}

bool UGClientNpcSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UGClientNpcSubsystem::OnGameInstanceInit()
{
	Super::OnGameInstanceInit();

	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UGClientNpcSubsystem::OnWorldBeginTearDown);
	{
		const TFunction<void(int32, float, FString)> Observer = [this](int32 LevelId, float LoadTime, FString MapName)
		{
			OnMapLoadComplete(LevelId, LoadTime, MapName);
		};
		EventObserverHolder.Observe(EGEventType::MapLoadComplete, Observer);
	}
}

void UGClientNpcSubsystem::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	if(!ClientNpcInfoMap.Num())
	{
		return;
	}
	
	AGPlayerCameraManager* CameraManager = UGCommonFunctions::GetPlayerCameraManager();
	if(!CameraManager)
	{
		return;
	}

	float DisappearDistanceSquared = FMath::Square(UGGameConfigSettings::Get()->ClientNpcDisappearDistance);
	float LoadDistanceSquared = FMath::Square(UGGameConfigSettings::Get()->ClientNpcLoadDistance);
	if(DisappearDistanceSquared < LoadDistanceSquared)
	{
		DisappearDistanceSquared = LoadDistanceSquared;
	}

	const FVector& CameraLocation = CameraManager->GetCameraComponentLocation();
	for (auto& KeyValue : ClientNpcInfoMap)
	{
		FGClientNpcInfo& ClientNpcInfo = KeyValue.Value;
		if(IsValid(ClientNpcInfo.NpcActor))
		{
			float DistanceSquared = FVector::DistSquared2D(ClientNpcInfo.NpcActor->GetActorLocation(), CameraLocation);
			if(DistanceSquared > DisappearDistanceSquared)
			{
				ClientNpcInfo.Location = ClientNpcInfo.NpcActor->GetActorLocation();
				ClientNpcInfo.NpcActor->Destroy();
				ClientNpcInfo.NpcActor = nullptr;
				ClientNpcInfo.NpcInfoComponent = nullptr;
			}
		}
		else
		{
			ClientNpcInfo.NpcActor = nullptr;
			float DistanceSquared = FVector::DistSquared2D(ClientNpcInfo.Location, CameraLocation);
			if(DistanceSquared < LoadDistanceSquared)
			{
				ClientNpcInfo.Spawn(GetWorld());
			}
		}
	}
}

int64 UGClientNpcSubsystem::CreateNpc(const FGNpcConfig& NpcConfig, bool bStandaloneServer, const TFunction<void(const FGClientNpcInfo&)>& AfterNpcSpawnFunction, FVector* Location)
{
	uint64 Guid = 0;
	if(bStandaloneServer)
	{
		Guid = UGGameInstance::Get()->GetGuidFactory().GetNextNpcGuid();
	}
	else
	{
		Guid = UGGameInstance::Get()->GetGuidFactory().GetNextClientNpcGuid();
	}

	FGClientNpcInfo& NpcInfo = ClientNpcInfoMap.Add(Guid);
	NpcInfo.NpcConfig = NpcConfig;
	if(Location)
		NpcInfo.Location = *Location;
	else
		NpcInfo.Location = NpcConfig.Transform.GetLocation();
	NpcInfo.AfterNpcSpawnFunction = AfterNpcSpawnFunction;

	//NpcInfo.Spawn(GetWorld());
	return Guid;
}

void UGClientNpcSubsystem::DestroyNpcByGuid(int64 Guid)
{
	if(FGClientNpcInfo* ClientNpcInfo = ClientNpcInfoMap.Find(Guid) )
	{
		if(IsValid(ClientNpcInfo->NpcActor))
		{
			ClientNpcInfo->NpcActor->Destroy();
		}
		ClientNpcInfoMap.Remove(Guid);
	}
}

void UGClientNpcSubsystem::DestroyNpcByConfigId(int NpcConfigId)
{
	int64 Guid = 0;
	for(const auto& KeyValue : ClientNpcInfoMap)
	{
		if(KeyValue.Value.NpcConfig.ConfigId != NpcConfigId)
		{
			continue;
		}
		Guid = KeyValue.Key;
	}
	DestroyNpcByGuid(Guid);
}

FGClientNpcInfo* UGClientNpcSubsystem::GetClientNpcInfo(int64 Guid)
{
	if (ClientNpcInfoMap.Find(Guid))
	{
		return &(ClientNpcInfoMap[Guid]);
	}

	return nullptr;
}

void UGClientNpcSubsystem::OnMapLoadComplete(int SceneId, float LoadTime, const FString& MapName)
{
	ClientNpcInfoMap.Empty();
}

void UGClientNpcSubsystem::OnWorldBeginTearDown(UWorld* InWorld)
{
	ClientNpcInfoMap.Empty();
}
