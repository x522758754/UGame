// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GSoundDef.generated.h"

/*
 * 音量大小
 */
USTRUCT(BlueprintType)
struct FGSoundData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float BasicVolumeRate;			//基础声音大小
	UPROPERTY(EditAnywhere)
	float MusicVolumeRate;			//背景音乐
	UPROPERTY(EditAnywhere)
	float AudioVolumeRate;			//场景音效
	UPROPERTY(EditAnywhere)
	bool bPlayMusic;				//是否播放背景音乐
	UPROPERTY(EditAnywhere)
	bool bPlayAudio;				//是否播放音效

	FGSoundData()
	{
		BasicVolumeRate = 1.0f;
		MusicVolumeRate = 1.0f;
		AudioVolumeRate = 1.0f;
		bPlayMusic = true;
		bPlayAudio = true;
	}
};