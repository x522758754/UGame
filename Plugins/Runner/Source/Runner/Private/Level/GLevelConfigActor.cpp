// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/GLevelConfigActor.h"

#include "Character/AI/GAIItem.h"

#include "Level/Data/GLevelNpcConfigAsset.h"
#include "Character/Npc/GNpcCharacter.h"
#include "Character/Npc/GNpcInfoComponent.h"
#include "Character/Npc/GNpcFunctions.h"
#include "Character/Data/GNpcData.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITOR
//#include "UObject/ObjectSaveContext.h"
#endif


#define UpperLimit 100000
#define FolderLimit 1000

// Sets default values
AGLevelConfigActor::AGLevelConfigActor():Super()
{
#if WITH_EDITOR
	if (GetLevel())
	{
		FCoreUObjectDelegates::OnObjectSaved.AddUObject(this, &AGLevelConfigActor::OnObjectPreSave);
	}
#endif

	bNetLoadOnClient = false;
}

TWeakObjectPtr<AGLevelConfigActor> AGLevelConfigActor::s_Instance;
AGLevelConfigActor* AGLevelConfigActor::Get(ULevel* Level)
{
	return s_Instance.Get();
}

void AGLevelConfigActor::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void AGLevelConfigActor::OnObjectPreSave(UObject* Object)
{
	if (GetLevel() != Object)
	{
		return;
	}

	SaveSceneConfig();
}


void AGLevelConfigActor::GenerateNpcConfiguration()
{
	ULevel* Level = GetLevel();
	if(!Level || !LevelNpcConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("AGSceneEditorActor::GenerateNpcConfiguration No SceneNpcConfig Or Level"));
		return;
	}
	TMap<int, FGNpcConfig>& NpcConfigMap = LevelNpcConfig->NpcConfigMap;
	NpcConfigMap.Empty();
	TArray<AActor*> NpcActorsNewly;
	
	TSet<FName> FolderPathNameSet;
	for (AActor* Actor : Level->Actors)
	{
		AGNpcCharacter* NpcInterface = Cast<AGNpcCharacter>(Actor);
		if(!NpcInterface)
		{
			continue;
		}

		FName FolderPathName = Actor->GetFolderPath();
		int FolderPrefix = GetFolderPrefix(FolderPathName);
		FolderPathNameSet.Add(FolderPathName);
		
		if(!Actor->IsHidden())
		{
			Actor->SetActorHiddenInGame(true);
			Actor->MarkPackageDirty();
		}
		NpcInterface->RefreshNpcConfig();	//重新生成NpcConfig
		UGNpcInfoComponent* InfoComponent = NpcInterface->GetInfoComponent<UGNpcInfoComponent>();
		if(!InfoComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("Error UGNpcInfoComponent %s"), *Actor->GetName());
			continue;
		}
		const FGNpcConfig& NpcConfig = InfoComponent->GetNpcConfig();
		int NpcConfigId = NpcConfig.ConfigId;

		if(NpcConfig.AIItem)
		{
			NpcConfig.AIItem->RefreshConfig(Actor);
		}
		
		
		if (((NpcConfigId % UpperLimit) / FolderLimit != FolderPrefix) || (NpcConfigId / UpperLimit != SceneId) || NpcConfigMap.Contains(NpcConfigId))
		{
			NpcActorsNewly.Add(Actor);
			continue;
		}
		NpcConfigMap.Add(NpcConfigId, UGNpcFunctions::DeepCopyNpcConfig(NpcConfig, LevelNpcConfig));
		
		UGNpcFunctions::SetNpcLabel(Actor, NpcConfigId, 0);
	}

	for (AActor* NpcActor : NpcActorsNewly)
	{
		//UGPawnFunctions::MarkLevelPackageDirty(NpcActor);
		FName FolderPathName = NpcActor->GetFolderPath();
		const int FolderPrefix = GetFolderPrefix(FolderPathName);
		int CurrentValidId = SceneId * UpperLimit + FolderPrefix * FolderLimit;
		CurrentValidId += 1;
		while (NpcConfigMap.Contains(CurrentValidId))
		{
			CurrentValidId += 1;
			continue;
		}
		AGNpcCharacter* NpcInterface = Cast<AGNpcCharacter>(NpcActor);
		UGNpcInfoComponent* InfoComponent = NpcInterface->GetInfoComponent<UGNpcInfoComponent>();
		InfoComponent->SetConfigId(CurrentValidId);
		UGNpcFunctions::SetNpcLabel(NpcActor, CurrentValidId, 0);
		LevelNpcConfig->NpcConfigMap.Add(CurrentValidId, UGNpcFunctions::DeepCopyNpcConfig(InfoComponent->GetNpcConfig(), LevelNpcConfig));
	}

	TSet<FName> DeletingFolder;
	for(const auto& KeyValue : FolderPrefixMap)
	{
		if(!FolderPathNameSet.Contains(KeyValue.Key))
		{
			DeletingFolder.Add(KeyValue.Key);
			MarkPackageDirty();
		}
	}
	for(const FName& Name : DeletingFolder)
	{
		FolderPrefixMap.Remove(Name);
	}

	LevelNpcConfig->MarkPackageDirty();
}

void AGLevelConfigActor::OnLevelActorListChanged()
{
	s_Instance = this;
	GEngine->OnLevelActorListChanged().RemoveAll(this);
}

bool AGLevelConfigActor::EqualSet(const TSet<int>& Set1, const TSet<int>& Set2)
{
	if(Set1.Num() != Set2.Num())
	{
		return false;
	}

	for(const int element : Set1)
	{
		if(!Set2.Contains(element))
		{
			return false;
		}
	}
	return true;
}
#endif

void AGLevelConfigActor::DoSave()
{
	SaveSceneConfig();
}

int AGLevelConfigActor::GetFolderPrefix(const FName& FolderPathName)
{
	if(FolderPathName.IsNone())
	{
		return 0;
	}
	int* Value = FolderPrefixMap.Find(FolderPathName);
	if(Value)
	{
		return *Value;
	}
	TSet<int> ExistValueSet;
	for(const auto& KeyValue : FolderPrefixMap)
	{
		ExistValueSet.Add(KeyValue.Value);
	}
	int NewValue = 1;
	while (NewValue < 99)
	{
		if(!ExistValueSet.Contains(NewValue))
		{
			break;
		}
		NewValue+=1;
	}
	if(NewValue > 99)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AGSceneConfigActor::GetFolderPrefix Too Much Folder"));
		return -1;
	}
	FolderPrefixMap.Add(FolderPathName, NewValue);
	MarkPackageDirty();
	return NewValue;
}

void AGLevelConfigActor::SaveSceneConfig()
{
#if WITH_EDITOR
	GenerateNpcConfiguration();
#endif
}
