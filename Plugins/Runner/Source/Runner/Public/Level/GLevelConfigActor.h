// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GLevelConfigActor.generated.h"

class UGLevelNpcConfigAsset;
/**
 *
 */
UCLASS(HideCategories=("Actor Tick","Replication","Rendering","Collision","Advanced","Input","HLOD","Events","Cooking","Physics","Networking"))
class AGLevelConfigActor : public AActor
{
	GENERATED_BODY()

	AGLevelConfigActor();

public:
	static AGLevelConfigActor* Get(ULevel* Level = nullptr);
private:
	static TWeakObjectPtr<AGLevelConfigActor> s_Instance;

#if WITH_EDITOR
private:
	void OnObjectPreSave(UObject* Object);

	bool EqualSet(const TSet<int>& Set1, const TSet<int>& Set2);
	void GenerateNpcConfiguration();
	void OnLevelActorListChanged();
#endif

	virtual bool ShouldTickIfViewportsOnly() const override {return true;}
	virtual void PostInitProperties() override;
public:
	UFUNCTION(CallInEditor, Category = "Editor")
	void DoSave();

private:
	int GetFolderPrefix(const FName& FolderPathName);
	void SaveSceneConfig();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SceneConfig")
	int SceneId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneConfig")
	UGLevelNpcConfigAsset* LevelNpcConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneConfig")
	TMap<FName, int> FolderPrefixMap;
};