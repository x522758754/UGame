// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GCameraDef.generated.h"

USTRUCT(BlueprintType)
struct FGCameraParam
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = -89.0, UIMax = 89.0))
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Yaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
};

USTRUCT(BlueprintType)
struct FGCameraFollowParam
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随目标", Category="相机跟随参数")
	TWeakObjectPtr<AActor> FollowActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随目标Socket", Category="相机跟随参数")
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随目标Socket", Category="相机跟随参数")
	float FollowSpeed;
};

USTRUCT(BlueprintType)
struct FGCameraConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="世界坐标系下最小俯仰角", Category="相机配置")
	float MinPitch = -89.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="世界坐标系下最大俯仰角", Category="相机配置")
	float MaxPitch = 89.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机Zoom最小距离", Category="相机配置")
	float MinZoomDistance = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机Zoom最大距离", Category="相机配置")
	float MaxZoomDistance = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随最近距离偏移", Category="相机跟随参数")
	FVector DeltaNearPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随最远距离偏移", Category="相机跟随参数")
	FVector DeltaFarPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机旋转速度", Category="相机配置")
	float RotateSpeed = 180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机速度", Category="相机配置")
	float ZoomSpeed = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机最小跟随速度", Category="相机配置")
	float MinFollowSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机最大跟随速度", Category="相机配置")
	float MaxFollowSpeed = 20000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随最小距离", Category="相机配置")
	float MinFollowDistance = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随最大距离", Category="相机配置")
	float MaxFollowDistance = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="相机跟随最大距离", Category="相机配置")
	bool IsIgnore;
};

USTRUCT(BlueprintType)
struct FGCameraCacheParam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsValid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGCameraFollowParam FollowParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGCameraParam CameraParam;

	//int32 CameraCfgId;
};