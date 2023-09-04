// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GNpcAIController.h"

#include "Character/Npc/GNpcInfoComponent.h"
#include "Character/AI/GAIItem.h"
#include "NavigationSystem.h"

AGNpcAIController::AGNpcAIController(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGNpcPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

bool AGNpcAIController::RunBehaviorTree(UBehaviorTree* BTAsset)
{
	if(NpcAIItem && NpcAIItem->IgnoreBehaviorTree())	//忽略行为树
	{
		return true;
	}
	return Super::RunBehaviorTree(BTAsset);
}

void AGNpcAIController::StopBehaviorTree()
{
	if(UBehaviorTreeComponent* BehaviorTreeComponent =  Cast<UBehaviorTreeComponent>( GetBrainComponent() ))
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AGNpcAIController::RestartBehaviorTree()
{
	if(UBehaviorTreeComponent* BehaviorTreeComponent =  Cast<UBehaviorTreeComponent>( GetBrainComponent() ))
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void AGNpcAIController::MoveCustom(const TArray<FVector>& PathPoint, bool IncludeCurrentLocation)
{
	if(PathPoint.Num() == 0)
	{
		return;
	}
	FAIMoveRequest MoveRequest(PathPoint[PathPoint.Num() - 1]);	
	
	FNavPathSharedPtr Path = MakeShared<FNavigationPath, ESPMode::ThreadSafe>();
	Path->MarkReady();
	TArray<FNavPathPoint>& NavPathPointArray = Path->GetPathPoints();
	FNavPathPoint NavPathPoint;
	if(IncludeCurrentLocation)
	{
		NavPathPoint.Location = GetPawn()->GetActorLocation();
		NavPathPointArray.Add(NavPathPoint);
	}
	for(const FVector Location : PathPoint)
	{
		NavPathPoint.Location = Location;
		NavPathPointArray.Add(NavPathPoint);
	}

	Path->DoneUpdating(ENavPathUpdateType::Type::Custom);
	RequestMove(MoveRequest, Path);
}

UGNpcPathFollowingComponent* AGNpcAIController::GetNpcPathFollowingComponent() const
{
	return Cast<UGNpcPathFollowingComponent>(GetPathFollowingComponent());
}

FPathFollowSegementDelegate& AGNpcAIController::GetPathSegmentFinishedDelegate()
{
	return GetNpcPathFollowingComponent()->FunctionSegmentFinished;
}

FPathFollowDelegate& AGNpcAIController::GetPathFollowResume()
{
	return GetNpcPathFollowingComponent()->FunctionPathFollowResume;
}

FPathFollowDelegate& AGNpcAIController::GetPathFinished()
{
	return GetNpcPathFollowingComponent()->FunctionPathFinished;
}

void AGNpcAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UGNpcInfoComponent* NpcInfoComponent = Cast<UGNpcInfoComponent>(InPawn->GetComponentByClass(UGNpcInfoComponent::StaticClass()));
	UGAIItem* ConfigItem = NpcInfoComponent->GetNpcConfig().AIItem;
	if(ConfigItem && ConfigItem->IsValid())
	{
		NpcAIItem = Cast<UGAIItem>(StaticDuplicateObject(ConfigItem, this));
		NpcAIItem->SetNpcAIController(this);
		NpcAIItem->Init(InPawn);
	}
}

void AGNpcAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AGNpcAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(NpcAIItem)
	{
		NpcAIItem->Tick(DeltaSeconds);
	}
}
