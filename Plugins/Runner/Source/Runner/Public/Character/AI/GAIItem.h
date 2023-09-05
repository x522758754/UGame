﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GAIItem.generated.h"


UCLASS(EditInlineNew,BlueprintType)
class UGAIItem : public UObject
{
	friend class AGNpcAIController;
	
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;

public:
	virtual const TSoftObjectPtr<UBehaviorTree>& GetBehaviorTree() {return BehaviorTree;}
public:
	virtual void RefreshConfig(AActor* NpcActor);
	virtual bool IsValid() { return true; }
	virtual bool IgnoreBehaviorTree() {return false;}
protected:
	virtual void Init(AActor* NpcActor) {};
	virtual void SetNpcAIController(AGNpcAIController* Controller);
	virtual void Tick(float DeltaSeconds){}
public:
	UFUNCTION()
	AGNpcAIController* GetNpcAIController() const;
protected:
	TWeakObjectPtr<AGNpcAIController> NpcAIController;
};