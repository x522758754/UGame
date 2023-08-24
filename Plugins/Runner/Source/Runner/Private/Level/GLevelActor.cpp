// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelActor.h"

#include "System/GAssetManager.h"
#include "Level/GLevelSubsystem.h"
#include "Level/Logic/GlevelLogicComponent.h"
// Sets default values
AGLevelActor::AGLevelActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGLevelActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->bShouldSimulatePhysics = bShouldSimulatePhysics;

	if(LogicClassPath.IsValid())
	{
		if(TSubclassOf<UGLevelLogicComponent> Class = UGAssetManager::LoadSubclass(LogicClassPath))
		{
			LevelLogicComponent = NewObject<UGLevelLogicComponent>(this, Class);
		}
	}
	else
	{
		LevelLogicComponent = NewObject<UGLevelLogicComponent>(this);
	}
	
	if(LevelLogicComponent)
	{
		LevelLogicComponent->RegisterComponent();
		LevelLogicComponent->SetIsReplicated(true);
		UE_LOG(LogTemp, Log, TEXT("AGSceneActor::BeginPlay Create SceneLogic %s"), *LevelLogicComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AGSceneActor::BeginPlay Create SceneLogic Failed"));
	}
}

void AGLevelActor::BeginDestroy()
{
	Super::BeginDestroy();
}

void AGLevelActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AGLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

