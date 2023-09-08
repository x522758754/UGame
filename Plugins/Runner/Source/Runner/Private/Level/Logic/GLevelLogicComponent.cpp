// Fill out your copyright notice in the Description page of Project Settings.
#include "Level/Logic/GLevelLogicComponent.h"

#include "Character/Npc/GClientNpcSubsystem.h"
#include "System/GCommonFunctions.h"
#include "Level/Data/GLevelNpcConfigAsset.h"
#include "Level/GLevelSubsystem.h"

// Sets default values for this component's properties
UGLevelLogicComponent::UGLevelLogicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...

	#if WITH_EDITOR
	if (GetOwner() && GetOwner()->GetLevel())
	{
		//FCoreUObjectDelegates::OnObjectSaved.AddUObject(this, &UGSceneLogicComponent::OnObjectSaved);
	}
#endif
}

// Called when the game starts
void UGLevelLogicComponent::BeginPlay()
{
	Super::BeginPlay();

	BeginInitializeScene();
}

// Called every frame
void UGLevelLogicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


bool UGLevelLogicComponent::ClientReady()
{
	return true;
}

void UGLevelLogicComponent::BeginInitializeScene()
{
	if(bIsSceneInitialize)
	{
		return;
	}
	bIsSceneInitialize = true;
	
	ProcessNeedSpawnActor();

	ProcessSpawnNpc();
}

void UGLevelLogicComponent::ProcessNeedSpawnActor()
{
	for (const auto& Value : NeedSpawnActorConfigArray)
	{
	}
}

void UGLevelLogicComponent::ProcessSpawnNpc()
{
	const UGLevelNpcConfigAsset* SceneNpcConfig = UGLevelSubsystem::Get()->GetCurrentLevelNpcConfigAsset();
	if(!SceneNpcConfig)
	{
		return;
	}

	UGClientNpcSubsystem* ClientNpcSubsystem = UGClientNpcSubsystem::Get();
	if(!ClientNpcSubsystem)
	{
		return;
	}
	//const bool Standalone = OwnerComponent->GetWorld()->GetNetMode() == NM_Standalone;
	const TMap<int, FGNpcConfig>& NpcConfigMap = SceneNpcConfig->NpcConfigMap;
	for (auto& KeyValue : NpcConfigMap)
	{
		const FGNpcConfig& NpcConfig = KeyValue.Value;
		if(!NpcConfig.AutoCreate)
		{
			continue;
		}
		
		if (NpcConfig.ClientOnly)
		{
			ClientNpcSubsystem->CreateNpc(NpcConfig, false);
		}
	}
}
