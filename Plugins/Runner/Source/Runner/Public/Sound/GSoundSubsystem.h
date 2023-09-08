// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Sound/GSoundDef.h"

#include "Components/AudioComponent.h"

#include "Event/GEventObserverHolder.h"

#include "System/GGameInstanceSubsystem.h"

#include "GSoundSubsystem.generated.h"

class AGAmbientSoundActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVolumeUpdateDelegate, float, volume);

USTRUCT()
struct FGAudioInfo
{
	GENERATED_BODY()

	UPROPERTY()
	UAudioComponent* AudioComponent;

	UPROPERTY()
	float VolumeMultiplier = 1;	//基础声音大小

	UPROPERTY()
	int64 Guid;

	UPROPERTY()
	USceneComponent* AttachComponent;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	bool bRecycle = true;
};


/**
 * 
 */
UCLASS()
class UGSoundSubsystem : public UGGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="SoundSubsystem")
	static UGSoundSubsystem* Get();
protected:
	static TWeakObjectPtr<UGSoundSubsystem> s_Instance;

public:
	virtual void OnGameInstanceInit() override;
	
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override {return  true;}
	virtual void OnTick(float DeltaTime) override;

public:
	//bg music
	void PlayMusic(USoundBase* Sound, bool bLoop = false, float StartTime = 0.f, float FadeInTime = 0.f, float FadeOutTime = 0.f);

public:
	//场景音乐音效
	UFUNCTION()
	int64 PlayAudioAtLocation(USoundBase* Sound, const FVector& Location, float BasicVolumeMultiplier = 1);
	UFUNCTION()
	int64 PlayAudioAtComponent(USoundBase* Sound, USceneComponent* Component, const FVector& Location, float BasicVolumeMultiplier = 1);
	UFUNCTION()
	void StopAudio(int64 AudioId);

	//音量设置
	UFUNCTION()
	void InitVolumeMultiplier();
	UFUNCTION()
	void SetMusicOpen(bool isOpen);
	UFUNCTION()
	bool GetMusicOpen();
	UFUNCTION()
	void SetAudioOpen(bool isOpen);

	void ReadSoundDataFromLocal();
	void SaveSoundDataToLocal();

	UPROPERTY(BlueprintAssignable)
	FVolumeUpdateDelegate OnVolumeUpdate;
	
	UFUNCTION(BlueprintCallable)
	void SetMusicVolumeMultiplier(float Value);

	UFUNCTION(BlueprintCallable)
	float GetMusicVolumeMultiplier() const;

	UFUNCTION()
	void SetAudioVolumeMultiplier(float Value);
	UFUNCTION(BlueprintCallable)
	float GetAudioVolumeMultiplier();

private:
	TWeakObjectPtr<UAudioComponent> MusicAudioComponent;
	//场景音乐音效
	AActor* GetAudioActor();
	UAudioComponent* GetValidAudioComponent();
	FGAudioInfo* CreateAudioInfo();

	//音量设置
	void UpdateVolumeMultiplier();

private:
	UPROPERTY(Transient)
	AActor* AudioOwnerActor;
	
	FGSoundData SoundData;
private:
	
	TMap<int64, FGAudioInfo> AudioInfoMap;
	UPROPERTY()
	TArray<UAudioComponent*> AudioComponentPool;
	int CurrentAudioGuid = 0;

	FGEventObserverHolder EventObserverHolder;
};
