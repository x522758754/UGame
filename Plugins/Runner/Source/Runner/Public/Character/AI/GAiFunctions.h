// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Kismet/BlueprintFunctionLibrary.h"
// #include "GAiFunctions.generated.h"
//
// class AGNpcAIController;
// /**
//  * 
//  */
// UCLASS()
// class UGAiFunctions : public UBlueprintFunctionLibrary
// {
// 	GENERATED_BODY()
//
//
// public:
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static int GetNearestPoint(const FVector& Current, const TArray<FVector>& Points);
//
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static int CheckNextPointIndex(const FVector& Current, const TArray<FVector>& Points, int CurrentIndex, float DistanceFar = 25);
//
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static int GetNextIndex(int CurrentIndex, int PointsNum, bool IsPatrolClosedLoop, bool IsReverseCurrent, bool& IsReverse);
//
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static void FetchSplineComponent(USplineComponent* SplineComponent, TArray<FVector>& Locations, bool& bIsClosedLoop);
//
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static void FetchSplineComponentWithDistance(USplineComponent* SplineComponent, TArray<FVector>& Locations, TMap<int, float> &WaitSecondsMap, bool& bIsClosedLoop, float GapDistance);
//
// 	UFUNCTION(BlueprintCallable, Category=GAiFunctions)
// 	static void CustomFetchSplineComponentWithDistance(USplineComponent* SplineComponent, TArray<FVector>& Locations, bool& bIsClosedLoop, float GapDistance);
// };
