// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Item/GAIChasePlayer.h"

#include "Actor/Router/GRouter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GNpcAIController.h"

void UGAIChasePlayer::RefreshConfig(AActor* NpcActor)
{
#if WITH_EDITOR
	Super::RefreshConfig(NpcActor);
	if(BehaviorTree.IsValid())
	{
		// if(UBehaviorTree* BT = BehaviorTree.LoadSynchronous())
		// {
		// 	if(UBlackboardData* BBData = BT->GetBlackboardAsset())
		// 	{
		// 		FBlackboardEntry *Entry = BBData->Keys.FindByPredicate([](FBlackboardEntry &Key)
		// 		{
		// 			return Key.EntryName == GET_MEMBER_NAME_CHECKED(UGAIChasePlayer, SightDistance);
		// 		});
		// 		if(!Entry)
		// 		{
		// 			FBlackboardEntry Entry_SightDistanceNew;
		// 			Entry_SightDistanceNew.EntryName = GET_MEMBER_NAME_CHECKED(UGAIChasePlayer, SightDistance);
		// 			Entry_SightDistanceNew.KeyType = NewObject<UBlackboardKeyType_Float>();
		// 			BBData->Keys.Add(Entry_SightDistanceNew);
		// 		}
		// 	}
		// }
	}
#endif
}

void UGAIChasePlayer::Init(AActor* NpcActor)
{
	if(UBlackboardComponent* BBComp = GetNpcAIController()->GetBlackboardComponent())
	{
		BBComp->SetValueAsFloat(GET_MEMBER_NAME_CHECKED(UGAIChasePlayer, SightDistance), SightDistance);
		BBComp->SetValueAsFloat(GET_MEMBER_NAME_CHECKED(UGAIChasePlayer, PatrolRadius), PatrolRadius);
	}
}

void UGAIChasePlayer::SetNpcAIController(AGNpcAIController* Controller)
{
	Super::SetNpcAIController(Controller);
}

void UGAIChasePlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void UGAIChasePlayer::OnSegmentFinished(int CurrentPathIndex)
{
}

void UGAIChasePlayer::OnPathFollowArrived(const FPathFollowingResult& Result)
{

}

void UGAIChasePlayer::OnResumePathFollow()
{
}

void UGAIChasePlayer::PatrolToTarget()
{

}
