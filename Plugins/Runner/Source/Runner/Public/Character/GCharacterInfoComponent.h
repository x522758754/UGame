// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/ActorChannel.h"
#include "GCharacterInfoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNNER_API UGCharacterInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGCharacterInfoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetConfigId(int32 InConfigId);
	virtual void SetGuid(int64 InGuid);

	int64 GetGuid();
	int32 GetConfigId();

protected:
	virtual void RegisterToSubsystem();
	virtual void UnregisterFromSubsystem();

protected://Replicated

	UFUNCTION()
	virtual void OnRep_ConfigId();

	UFUNCTION()
	virtual void OnRep_Guid();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterInfo", replicatedUsing = OnRep_ConfigId)
	int32 ConfigId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterInfo", replicatedUsing = OnRep_Guid)
	int64 Guid;
		
};
