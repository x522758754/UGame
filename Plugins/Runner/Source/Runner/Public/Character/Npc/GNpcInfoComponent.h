// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GCharacterInfoComponent.h"
#include "Character/Data/GNpcData.h"
#include "GNpcInfoComponent.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGNpcInfoComponent : public UGCharacterInfoComponent
{
	GENERATED_BODY()

public:
	virtual void SetConfigId(int32 InConfigId) override;
	void InitNpcConfig(const FGNpcConfig& InNpcConfig);
	
	const FGNpcConfig& GetNpcConfig() { return NpcConfig; }
#if WITH_EDITOR
	FGNpcConfig& GetMutableNpcConfig() { return NpcConfig; }
#endif

protected:
	virtual void ApplyNpcConfig();

	void AcquireNpcConfig();
	
	virtual void RegisterToSubsystem() override;
	virtual void OnRep_ConfigId() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FGNpcConfig NpcConfig;
};
