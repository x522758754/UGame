// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Character/Component/GNpcPathFollowingComponent.h"

#include "GNpcAIController.generated.h"

class UGAIItem;
/**
 * 
 */
UCLASS()
class RUNNER_API AGNpcAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGNpcAIController(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset) override;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "NpcAIController")
	void StopBehaviorTree();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "NpcAIController")
	void RestartBehaviorTree();
	UFUNCTION(BlueprintCallable)
	void MoveCustom(const TArray<FVector>& PathPoint, bool IncludeCurrentLocation = true);
	UGNpcPathFollowingComponent* GetNpcPathFollowingComponent() const;
	FPathFollowSegementDelegate& GetPathSegmentFinishedDelegate();
	FPathFollowDelegate& GetPathFollowResume();
	FPathFollowDelegate& GetPathFinished();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;
public:

	UPROPERTY()
	UGAIItem* NpcAIItem;
};
