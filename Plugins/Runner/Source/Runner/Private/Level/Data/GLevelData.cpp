// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Data/GLevelData.h"
#include "System/GAssetManager.h"

FString FGLevelConfig::GetMapName() const
{
	FString Path = FPackageName::ObjectPathToPackageName(MapLevel.ToString());
	//return MapLevel.ToString()
	
	return Path;
}

FString FGLevelConfig::ConstructTravelURL() const
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
