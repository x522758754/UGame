// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GAssetManager.generated.h"

//FSoftObjectPath 能放入UE的任何资源,tostring的路径就是相对游戏的路径,SoftObjectPathtostring=/Game/UMG/LoginUI.LoginUI
//FSoftClassPath 只能放入蓝图类,tostring的路径结尾_C,SoftClasstPathtostring=/Game/UMG/LoginUI.LoginUI_C

class UGConfigData;

enum class EGLoadPriority : int32
{
	Normal = 0,
	High = 10,
};

/**
 * 
 */
UCLASS()
class RUNNER_API UGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UGAssetManager* Get();

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;

	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static AssetType* LoadAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static TSubclassOf<AssetType> LoadSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template<typename AssetType>
	static void AsyncLoadAsset(const TSoftObjectPtr<AssetType>& AssetPointer, TFunction<void(AssetType*)> Callback = nullptr, EGLoadPriority Priority = EGLoadPriority::Normal, bool bKeepInMemory = true);

	template<typename AssetType>
	static void AsyncLoadSubclass(const TSoftClassPtr<AssetType>& AssetPointer, TFunction<void(TSubclassOf<AssetType>)> Callback = nullptr, EGLoadPriority Priority = EGLoadPriority::Normal, bool bKeepInMemory = true);

	UGConfigData* GetConfigData();

protected:
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	static TSharedPtr<FStreamableHandle> AsyncLoadAsset(const FSoftObjectPath& AssetPath, TFunction<void(UObject*)> Callback = nullptr, EGLoadPriority Priority = EGLoadPriority::Normal);

	static bool ShouldLogAssetLoads();
	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);

protected:
	UPROPERTY()
	UGConfigData* ConfigData;

private:
	
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<const UObject*> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UGAssetManager::LoadAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get()->AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UGAssetManager::LoadSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get()->AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}

template <typename AssetType>
void UGAssetManager::AsyncLoadAsset(const TSoftObjectPtr<AssetType>& AssetPointer, TFunction<void(AssetType*)> Callback, EGLoadPriority Priority, bool bKeepInMemory)
{
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		AssetType* LoadedAsset = AssetPointer.Get();
		if(LoadedAsset)
		{
			if(Callback)
			{
				Callback(LoadedAsset);
			}
			return;
		}
	}
	TSharedPtr<FStreamableHandle> Request = AsyncLoadAsset(AssetPath, Callback, Priority);
	FStreamableDelegate Delegate;
	Delegate.BindLambda([Request, Callback, AssetPointer, bKeepInMemory]()
	{
		AssetType* LoadedAsset = Cast<AssetType>(Request->GetLoadedAsset());
		ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		
		if(Callback)
		{
			Callback(LoadedAsset);
		}
		
		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get()->AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	});
	Request->BindCompleteDelegate(MoveTemp(Delegate));
}

template <typename AssetType>
void UGAssetManager::AsyncLoadSubclass(const TSoftClassPtr<AssetType>& AssetPointer, TFunction<void(TSubclassOf<AssetType>)> Callback, EGLoadPriority Priority, bool bKeepInMemory)
{
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		TSubclassOf<AssetType> LoadedSubclass = AssetPointer.Get();
		if(LoadedSubclass)
		{
			if(Callback)
			{
				Callback(LoadedSubclass);
			}
			return;
		}
	}
	TSharedPtr<FStreamableHandle> Request = AsyncLoadAsset(AssetPath, Callback, Priority);
	FStreamableDelegate Delegate;
	Delegate.BindLambda([Request, Callback, AssetPointer, bKeepInMemory]()
	{
		TSubclassOf<AssetType> LoadedAsset = Cast<UClass>(Request->GetLoadedAsset());
		ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		
		if(Callback)
		{
			Callback(LoadedAsset);
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get()->AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	});
	Request->BindCompleteDelegate(MoveTemp(Delegate));
}
