// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/GCameraDef.h"
#include "GPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AGPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AGPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	void SetFollow(AActor* Target, FName SocketName = NAME_None, float FollowSpeed = -1.f);
	AActor* GetFollow();
	FVector GetPivotLocation(); //获取实际跟随点

	void SetConfig(int32 CfgId);
	int32 GetConfig() const;

	void CacheParam(bool bForce);
	void ApplyCacheParam(bool bImmediately = true);
	void ClearCacheParam();

	void SetCameraParam(const FGCameraParam& Param, bool bImmediately = false);
	void SetRoll(float Roll, bool bImmediately = false);
	void SetPitch(float Pitch, bool bImmediately = false);
	void SetYaw(float Yaw, bool bImmediately = false);
	void SetDistance(float Distance, bool bImmediately = false);
	float GetPitch(bool bImmediately = true);
	float GetYaw(bool bImmediately = true);
	float GetDistance(bool bImmediately = true);

	void Immediately();

protected:
	void TickFollow(float DeltaSeconds);
	void TickParam(float DeltaSeconds);
	void RefreshParam();
	bool LerpParameter(float& Current, float Target, float Speed, float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<ACameraActor> ActiveCamera;
	UPROPERTY(EditAnywhere)
	FGCameraFollowParam FollowParam;
	UPROPERTY(EditAnywhere)
	FGCameraConfig Config;
	UPROPERTY(EditAnywhere)
	FGCameraParam CurrentParam;
	UPROPERTY(EditAnywhere)
	FGCameraParam TargetParam;
	UPROPERTY(EditAnywhere)
	FGCameraCacheParam CachedParam;
	UPROPERTY(EditAnywhere)
	bool ParamDirty;

private:
	int32 CameraCfgId;
};
