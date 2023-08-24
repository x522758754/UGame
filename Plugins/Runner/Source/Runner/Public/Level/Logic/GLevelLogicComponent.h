// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event/GEventObserverHolder.h"
#include "Components/ActorComponent.h"
#include "GLevelLogicComponent.generated.h"


USTRUCT(BlueprintType)
struct FGSceneBornCircleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Yaw = 0;
};

USTRUCT(BlueprintType)
struct FGSceneNeedSpawnActorConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftClassPath ActorSoftClassPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SpawnInServer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SpawnInClient = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorSpawnTransform;
};

UCLASS(Blueprintable, ClassGroup=(Scene), meta=(BlueprintSpawnableComponent) )
class UGLevelLogicComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGLevelLogicComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual bool ClientReady();
	
	bool IsSceneInitialize() const {return bIsSceneInitialize;}
	virtual void BeginInitializeScene();	//场景加载完成后，开始初始化场景
private:
	void ProcessNeedSpawnActor();
	void ProcessSpawnNpc();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config BornTransform")
	TArray<FGSceneBornCircleConfig> SceneBornConfigs;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config Basic")
	TArray<FGSceneNeedSpawnActorConfig> NeedSpawnActorConfigArray;
private:
	bool bIsSceneInitialize = false;
protected:
	FGEventObserverHolder EventObserverHolder;
};
