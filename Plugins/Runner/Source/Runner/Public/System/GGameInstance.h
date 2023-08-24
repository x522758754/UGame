// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/GGuidFactory.h"
#include "GGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static UGGameInstance* Get();

	UGGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	FGGuidFactory& GetGuidFactory() {return GuidFactory;}

protected:

	virtual void Init() override;
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

	virtual TStatId GetStatId() const override;
	virtual void Tick( float DeltaTime ) override;
protected:
	static TWeakObjectPtr<UGGameInstance> GameInstancePtr;

private:
	FGGuidFactory GuidFactory;
};
