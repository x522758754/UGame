// Fill out your copyright notice in the Description page of Project Settings.
#include "Sound/GSoundSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Level/GLevelSubsystem.h"
#include "Level/Data/GLevelData.h"

#include "SaveGame/GClientSaveGame.h"
#include "SaveGame/GSaveGameSubsystem.h"

#include "System/GAssetManager.h"
#include "System/GGameInstance.h"

#define G_VolumeMultiplier_MinValue 0.0002f

TWeakObjectPtr<UGSoundSubsystem> UGSoundSubsystem::s_Instance;
UGSoundSubsystem* UGSoundSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGSoundSubsystem>();
	}
	return s_Instance.Get();
}

void UGSoundSubsystem::OnGameInstanceInit()
{
	const TFunction<void(int32)> ObserverLevelLoadEnd = [this](int32 LevelId)
	{
		if(const FGLevelConfig* LevelConfig = UGLevelSubsystem::Get()->GetCurrentLevelConfig())
		{
			if(USoundBase* SoundBase = UGAssetManager::LoadAsset(LevelConfig->SoundAsset))
			{
				PlayMusic(SoundBase, true, 0.f, 0.2f, 0.2f);	
			}
		}
	};
	EventObserverHolder.Observe(EGEventType::LevelLoadEnd, ObserverLevelLoadEnd);
}

void UGSoundSubsystem::OnTick(float DeltaTime)
{
	static TArray<int64> s_RemoveKey;
	s_RemoveKey.Empty(s_RemoveKey.Num());
	for (const auto& KeyValue : AudioInfoMap)
	{
		const FGAudioInfo& Info = KeyValue.Value;
		if (Info.AttachComponent)
		{
			Info.AudioComponent->SetWorldLocation(Info.AttachComponent->GetComponentLocation() + Info.Location);
		}
		if (!Info.AudioComponent->IsPlaying())
		{
			s_RemoveKey.Add(KeyValue.Key);
		}
	}
	for (const int64& Key : s_RemoveKey)
	{
		FGAudioInfo* AudioInfo = AudioInfoMap.Find(Key);
		if (AudioInfo->bRecycle)
		{
			AudioComponentPool.Add(AudioInfo->AudioComponent);
			AudioInfoMap.Remove(Key);
		}
	}
}

void UGSoundSubsystem::PlayMusic(USoundBase* Sound, bool bLoop, float StartTime, float FadeInTime, float FadeOutTime)
{
	if(Sound)
	{
		if(USoundWave* SoundWave = Cast<USoundWave>(Sound))
		{
			SoundWave->bLooping = bLoop;	
		}
		if(!MusicAudioComponent.IsValid())
		{
			MusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), Sound);
		}
		else
		{
			// todo
			// if(FadeOutTime > 0.f)
			// {
			// 	MusicAudioComponent->FadeOut(FadeOutTime, GetMusicVolumeMultiplier());
			// }
			// else
			{
				MusicAudioComponent->Stop();
			}
		}
		if(MusicAudioComponent.IsValid())
		{
			MusicAudioComponent->SetSound(Sound);
			MusicAudioComponent->SetAutoActivate(false);
			
			if(FadeInTime > 0.f)
			{
				MusicAudioComponent->FadeIn(FadeInTime, GetMusicVolumeMultiplier(), StartTime);
			}
			else
			{
				MusicAudioComponent->Play(StartTime);
				MusicAudioComponent->SetVolumeMultiplier(GetMusicVolumeMultiplier());
			}
		}
		
	}
}

AActor* UGSoundSubsystem::GetAudioActor()
{
	if (AudioOwnerActor == nullptr)
	{
		AudioOwnerActor = GetGameInstance()->GetWorld()->SpawnActor<AActor>();
	}

	return AudioOwnerActor;
}

UAudioComponent* UGSoundSubsystem::GetValidAudioComponent()
{
	if (AudioComponentPool.Num() > 0)
	{
		UAudioComponent* AudioComponent = AudioComponentPool[AudioComponentPool.Num() - 1];
		if (AudioComponent)
		{
			AudioComponentPool.RemoveAtSwap(AudioComponentPool.Num() - 1);
			return AudioComponent;
		}
	}
	AActor* Parent = GetAudioActor();
	if (!Parent)
	{
		return nullptr;
	}
	UAudioComponent* AudioComponent = NewObject<UAudioComponent>(Parent, UAudioComponent::StaticClass());
	return AudioComponent;
}

FGAudioInfo* UGSoundSubsystem::CreateAudioInfo()
{
	UAudioComponent* AudioComponent = GetValidAudioComponent();
	if (!AudioComponent)
	{
		return nullptr;
	}
	CurrentAudioGuid++;
	FGAudioInfo& Ref = AudioInfoMap.Add(CurrentAudioGuid, FGAudioInfo());

	Ref.Guid = CurrentAudioGuid;
	Ref.AudioComponent = AudioComponent;
	return &Ref;
}


int64 UGSoundSubsystem::PlayAudioAtLocation(USoundBase* Sound, const FVector& Location, float BasicVolumeMultiplier)
{
	return PlayAudioAtComponent(Sound, nullptr, Location, BasicVolumeMultiplier);
}

int64 UGSoundSubsystem::PlayAudioAtComponent(USoundBase* Sound, USceneComponent* Component, const FVector& Location, float BasicVolumeMultiplier)
{
	FGAudioInfo* AudioInfo = CreateAudioInfo();
	if(!AudioInfo)
	{
		return 0;
	}
	AudioInfo->AttachComponent = Component;
	AudioInfo->Location = Location;
	AudioInfo->VolumeMultiplier = BasicVolumeMultiplier;
	
	UAudioComponent* AudioComponent = AudioInfo->AudioComponent;
	if(Component)
	{
		AudioComponent->SetWorldLocation(Component->GetComponentLocation() + Location);
	}
	else
	{
		AudioComponent->SetWorldLocation(Location);
	}
	AudioComponent->Sound = Sound;
	AudioComponent->bAutoActivate = false;
	AudioComponent->VolumeMultiplier = BasicVolumeMultiplier * GetAudioVolumeMultiplier();
	AudioComponent->Play();
	return AudioInfo->Guid;
}

void UGSoundSubsystem::StopAudio(int64 AudioId)
{
	if (AudioInfoMap.Find(AudioId))
	{
		AudioInfoMap[AudioId].AudioComponent->Stop();
	}
}

//--------------------------------------------------------------音量设置-----------------------------------------------------------
void UGSoundSubsystem::InitVolumeMultiplier()
{
	ReadSoundDataFromLocal();
	UpdateVolumeMultiplier();
}

void UGSoundSubsystem::SetMusicOpen(bool isOpen)
{
	SoundData.bPlayMusic = isOpen;
	SaveSoundDataToLocal();
	UpdateVolumeMultiplier();
}

bool UGSoundSubsystem::GetMusicOpen()
{
	return SoundData.bPlayMusic;
}

void UGSoundSubsystem::SetAudioOpen(bool isOpen)
{
	SoundData.bPlayAudio = isOpen;
	SaveSoundDataToLocal();
	UpdateVolumeMultiplier();
}

void UGSoundSubsystem::SetMusicVolumeMultiplier(float Value)
{
	SoundData.MusicVolumeRate = FMath::Clamp(Value, 0.f, 1.f);

	UpdateVolumeMultiplier();
}

float UGSoundSubsystem::GetMusicVolumeMultiplier() const
{
	float Volume = 0;
	if(SoundData.bPlayMusic)
	{
		Volume = SoundData.BasicVolumeRate * SoundData.MusicVolumeRate;
	}
	return FMath::Max(Volume, G_VolumeMultiplier_MinValue);
}

void UGSoundSubsystem::SetAudioVolumeMultiplier(float Value)
{
	SoundData.AudioVolumeRate = FMath::Clamp(Value, 0.01f, 1.0f);
	SaveSoundDataToLocal();
	UpdateVolumeMultiplier();
}

float UGSoundSubsystem::GetAudioVolumeMultiplier()
{
	float Volume = 0;
	if(SoundData.bPlayAudio)
	{
		Volume = SoundData.BasicVolumeRate * SoundData.AudioVolumeRate;
	}
	return FMath::Max(Volume, G_VolumeMultiplier_MinValue);
}

void UGSoundSubsystem::UpdateVolumeMultiplier()
{
	const float AudioVolumeRate = GetAudioVolumeMultiplier();
	for (const auto& KeyValue : AudioInfoMap)
	{
		const FGAudioInfo& Info = KeyValue.Value;
		if (Info.AudioComponent)
		{
			Info.AudioComponent->SetVolumeMultiplier(AudioVolumeRate * Info.VolumeMultiplier);
		}
	}
	
	const float MusicVolume = GetMusicVolumeMultiplier();
	if(MusicAudioComponent.IsValid())
	{
		MusicAudioComponent->SetVolumeMultiplier(MusicVolume);
	}
}

void UGSoundSubsystem::ReadSoundDataFromLocal()
{
	if (UGClientSaveGame* saveGame = UGSaveGameSubsystem::Get()->GetClientSaveGame())
	{
		SoundData = saveGame->SettingSoundData;
	}
}

void UGSoundSubsystem::SaveSoundDataToLocal()
{
	if (UGClientSaveGame* saveGame = UGSaveGameSubsystem::Get()->GetClientSaveGame())
	{
		UGSaveGameSubsystem::Get()->SaveSettingData(SoundData);
	}
}