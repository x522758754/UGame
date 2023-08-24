// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelFunctions.h"

#include "System/GAssetManager.h"
#include "Level/Data/GLevelNpcConfigAsset.h"
#include "Level/GLevelSubsystem.h"
#include "Config/GConfigDataAsset.h"


int32 UGLevelFunctions::GetLevelId(const FString& MapName)
{
	for(auto It = UGConfigDataAsset::Get()->LevelConfigs.CreateConstIterator(); It; ++It)
	{
		if(It.Value().GetMapName() == MapName)
		{
			return It.Key();
		}
	}

	return 0;
}

bool UGLevelFunctions::FitLocation(FVector& Location, float CapsuleHalfHeight, UWorld* World, float ExtendLength)
{
	if(ExtendLength <= 0)
	{
		ExtendLength = CapsuleHalfHeight * 2;
	}
	FVector Start = Location + FVector(0, 0, ExtendLength);
	FVector End = Location - FVector(0, 0, ExtendLength);
	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.bTraceComplex = true;
	if (!World->LineTraceSingleByObjectType(HitResult, Start, End, ECC_WorldStatic))
	{
		End = Start;
		Start += FVector(0, 0, CapsuleHalfHeight * 2);
		if (World->LineTraceSingleByObjectType(HitResult, Start, End, ECC_WorldStatic))
		{
			Location = HitResult.Location;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Location = HitResult.Location;
	}
	Location.Z += CapsuleHalfHeight;
	return true;
}

const FGNpcConfig* UGLevelFunctions::GetNpcConfig(int NpcConfigId)
{
	if(UGLevelNpcConfigAsset* LevelNpcConfigAsset = UGLevelSubsystem::Get()->GetCurrentLevelNpcConfigAsset())
	{
		if(LevelNpcConfigAsset->NpcConfigMap.Contains(NpcConfigId))
		{
			return &LevelNpcConfigAsset->NpcConfigMap[NpcConfigId];
		}
	}
	
	return nullptr;
}