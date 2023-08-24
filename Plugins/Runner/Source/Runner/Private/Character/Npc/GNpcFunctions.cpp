// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Npc/GNpcFunctions.h"

#include "System/GAssetManager.h"
#include "Level/GLevelFunctions.h"
#include "System/GGameInstance.h"
#include "Character/Npc/GNpcInfoComponent.h"
#include "Character/Npc/GNpcCharacter.h"
#include "Components/CapsuleComponent.h"

FGNpcConfig UGNpcFunctions::DeepCopyNpcConfig(const FGNpcConfig& NpcConfig, UObject* Outer)
{
	FGNpcConfig Result = NpcConfig;
	// if(NpcConfig.AI_Item)
	// {
	// 	Result.AI_Item = Cast<UGNpcAI_Base>(StaticDuplicateObject(NpcConfig.AI_Item, Outer));
	// }
	// if(NpcConfig.NpcActorItem)
	// {
	// 	Result.NpcActorItem = Cast<UGNpcActorItem>(StaticDuplicateObject(NpcConfig.NpcActorItem, Outer));
	// }
	// Result.Interactive.InteractiveItems.Empty();
	// for(const UGInteractiveItem* InteractiveItem : NpcConfig.Interactive.InteractiveItems)
	// {
	// 	if(InteractiveItem)
	// 	{
	// 		Result.Interactive.InteractiveItems.Add(Cast<UGInteractiveItem>(StaticDuplicateObject(InteractiveItem, Outer)));
	// 	}
	// }
	
	return Result;
}

void UGNpcFunctions::SetNpcLabel(AActor* NpcActor, int NpcConfigId, int64 Guid)
{
#if WITH_EDITOR
	if(NpcActor)
	{
		if(Guid)
		{
			NpcActor->SetActorLabel(FString::Printf(TEXT("%s_%d_%llx"), *NpcActor->GetClass()->GetName(), NpcConfigId, Guid) );
		}
		else
		{
			NpcActor->SetActorLabel(FString::Printf(TEXT("%s_%d"), *NpcActor->GetClass()->GetName(), NpcConfigId) );
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGPawnFunctions::SetNpcLabel NoneActor %lld=%d"), Guid, NpcConfigId);
	}
#endif
}

UGNpcInfoComponent* UGNpcFunctions::Client_SpawnNpc(const FGNpcConfig& NpcConfig, UWorld* World)
{
	UGNpcInfoComponent* NpcInfoComponent = nullptr;
	switch (NpcConfig.NpcType)
	{
	case EGNpcType::Character:
		NpcInfoComponent = Client_SpawnNpcCharacter(NpcConfig, World);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UGNpcFunctions::Client_SpawnNpc Error NpcType(%d) Config=%d"),
			   static_cast<int>( NpcConfig.NpcType ), NpcConfig.ConfigId);
		return nullptr;
	}
	if(!NpcInfoComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("UGNpcFunctions::Client_SpawnNpc Failed NpcType(%d) Config=%d"),  static_cast<int>( NpcConfig.NpcType ), NpcConfig.ConfigId);
		return nullptr;
	}
	NpcInfoComponent->InitNpcConfig(NpcConfig);
	return NpcInfoComponent;
}

UGNpcInfoComponent* UGNpcFunctions::Client_SpawnNpcCharacter(const FGNpcConfig& NpcConfig, UWorld* World)
{
	AGNpcCharacter* NpcCharacter = SpawnNpcCharacter(NpcConfig, World);
	if(!NpcCharacter)
	{
		return nullptr;
	}
	return NpcCharacter->GetInfoComponent<UGNpcInfoComponent>();;
}

AGNpcCharacter* UGNpcFunctions::SpawnNpcCharacter(const FGNpcConfig& NpcConfig, UWorld* World)
{
	TSubclassOf<AGNpcCharacter> NpcClass = UGAssetManager::LoadSubclass(NpcConfig.NpcClass);
	if(NpcClass == nullptr)
	{
		return nullptr;
	}
	AGNpcCharacter* NpcCharacter = World->SpawnActor<AGNpcCharacter>(NpcClass);
	if (!NpcCharacter)
	{
		return nullptr;
	}
	
	NpcCharacter->SetActorTransform(NpcConfig.Transform);
	if(NpcConfig.CreateOnGround)
	{
		FVector Location = NpcConfig.Transform.GetLocation();
		if(!UGLevelFunctions::FitLocation(Location, NpcCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2, World ) )
		{
			UE_LOG(LogTemp, Error, TEXT("UGSceneFunctions::SpawnNpcCharacter Error ConfigId(%d) Location %s"), NpcConfig.ConfigId, *Location.ToString());
			return NpcCharacter;
		}
	}
	return NpcCharacter;
}
