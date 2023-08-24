// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GGameInstanceSubsystem.h"
#include "GHeroSubsystem.generated.h"

class AGHeroCharacter;
class UGHeroInfoComponent;

/**
 * 
 */
UCLASS()
class RUNNER_API UGHeroSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION()
	static UGHeroSubsystem* Get();
protected:
	static TWeakObjectPtr<UGHeroSubsystem> s_Instance;

public:
	void AddInfoComponent(UGHeroInfoComponent* Component);
	void RemoveInfoComponent(UGHeroInfoComponent* Component);
public:
	UGHeroInfoComponent* GetInfoComponent(int64 Guid);
	AGHeroCharacter* GetHeroCharacter(int64 Guid);

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnGameInstanceInit() override;
	virtual void OnTick(float DeltaTime) override;

protected:
	UPROPERTY(Transient)
	TMap<int64, UGHeroInfoComponent*> InfoMap;
};


