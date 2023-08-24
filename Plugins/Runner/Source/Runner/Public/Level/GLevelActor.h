// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GLevelActor.generated.h"

class UGLevelLogicComponent;

/*
 * Level Actor，同步场景信息和管理场景数据
 */
UCLASS()
class AGLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGLevelActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UGLevelLogicComponent* GetLevelLogicComponent() const {return LevelLogicComponent;}

	template<class T>
	T* GetLevelLogicComponent()
	{
		return Cast<T>(GetLevelLogicComponent());
	}
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config")
	bool bShouldSimulatePhysics = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UGLevelLogicComponent> LogicClassPath;
private:
	UPROPERTY()
	UGLevelLogicComponent* LevelLogicComponent;


};
