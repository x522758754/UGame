// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static UGGameInstance* Get();

	UGGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init() override;
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
protected:
	static TWeakObjectPtr<UGGameInstance> GameInstancePtr;
};
