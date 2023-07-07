// Copyright 2020 Dan Kestranek.


#include "System/GAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/GGameplayTags.h"


UGAssetManager* UGAssetManager::Get() 
{
	UGAssetManager* Singleton = Cast<UGAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be GDAssetManager!"));
		return NewObject<UGAssetManager>();	 // never calls this
	}
}


void UGAssetManager::StartInitialLoading() 
{
	Super::StartInitialLoading();

	FGGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

UObject* UGAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (IsValid())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UGAssetManager::ShouldLogAssetLoads()
{
	return false;
}

void UGAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
