// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Character/Component/GPathFollowingComponent.h"
#include "AITypes.h"
#include "NavigationData.h"
#include "GNavigateFunctions.generated.h"

/**
 * 参考 AAIController::MoveToLocation 、AAIController::MoveTo
 *
 *	控制Pawn移动的真正的核心在于AAIController::MoveTo：
利用NavSys->ProjectPointToNavigation将需要导航的位置投影到导航系统中；
通过MoveRequest.UpdateGoalLocation(ProjectedLocation.Location); 更新FAIMoveRequest中的目标点为投影后的点；
利用BuildPathfindingQuery(MoveRequest, PFQuery)为一个FAIMoveRequest创建查询；
FindPathForMoveRequest(MoveRequest, PFQuery, Path)获取查询得到的路径；
路径查询成功利用RequestMove(MoveRequest, Path) 执行移动操作。
 */
UCLASS()
class  UGNavigateFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	static UGPathFollowingComponent* GetPathFollowingComponent();

	static FAIRequestID NavigateToGoal(const FVector& GoalLocation, const TFunction<void(FAIRequestID, const FPathFollowingResult& Result)>& OnRequestFinished = nullptr);

	static void AbortMove();

	static bool IsNavigating();

	static bool IsNavigatingToGoalLocation(const FVector& GoalLocation);

	static bool IsNavigatingToGoal(AActor* Goal);

	static FPathFindingResult GetPathFindingResult(const FVector& StartLocation, const FVector& GoalLocation, bool IsUsingPartialPaths = true);
	
	static bool CanNavigateToLocation(const FVector& GoalLocation);

	static bool GetNavigateToGoalLocation(TArray<FNavPathPoint>& PathPoints, const FVector& GoalLocation);
};



