// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Data/GLevelData.h"
#include "System/GAssetManager.h"

FString UGLevelData::GetMapName() const
{
	FAssetData MapAssetData;
	if (UGAssetManager::Get()->GetPrimaryAssetData(MapID, /*out*/ MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	else
	{
		return FString();
	}
}

FString UGLevelData::ConstructTravelURL() const
{
	FString CombinedExtraArgs;
	
	for (const auto& KVP : ExtraArgs)
	{
		if (!KVP.Key.IsEmpty())
		{
			if (KVP.Value.IsEmpty())
			{
				CombinedExtraArgs += FString::Printf(TEXT("?%s"), *KVP.Key);
			}
			else
			{
				CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *KVP.Key, *KVP.Value);
			}
		}
	}

	return FString::Printf(TEXT("%s%s"),
		*GetMapName(),
		*CombinedExtraArgs);
}
