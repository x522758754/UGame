// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/GPlayerCameraManager.h"

#include "System/GAssetManager.h"
#include "Config/GGameConfigSettings.h"
#include "Camera/GCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void CullDeg(float& v)
{
	if (v > 360.0f || v < -360.0f)
	{
		int32_t t = (int32_t)(v / 360.0f);
		v -= ((float)t * 360.0f);
	}
}

AGPlayerCameraManager::AGPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(TSubclassOf<AGCameraActor> CameraClass = UGAssetManager::LoadSubclass(UGGameConfigSettings::Get()->Camera))
	{
		ActiveCamera = GetWorld()->SpawnActor<AGCameraActor>(CameraClass, SpawnInfo);
	}
	else
	{
		ActiveCamera = GetWorld()->SpawnActor<AGCameraActor>(AGCameraActor::StaticClass(), SpawnInfo);
	}
	SetViewTarget(ActiveCamera.Get());
}

void AGPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickFollow(DeltaSeconds);
	TickParam(DeltaSeconds);
}

void AGPlayerCameraManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(AGPlayerCameraManager, CurrentParam))
	{
		TargetParam = CurrentParam;
		RefreshParam();
	}

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(AGPlayerCameraManager, Config))
	{
		RefreshParam();
	}

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(AGPlayerCameraManager, FollowParam))
	{
		RefreshParam();
	}
	
}

void AGPlayerCameraManager::SetFollow(AActor* Target, FName SocketName, float FollowSpeed)
{
	FollowParam.FollowActor = Target;
	FollowParam.SocketName = SocketName;
	FollowParam.FollowSpeed = FollowSpeed;
	
	ParamDirty = true;
}

AActor* AGPlayerCameraManager::GetFollow()
{
	return FollowParam.FollowActor.Get();
}

FVector AGPlayerCameraManager::GetPivotLocation()
{
	FVector PivotLoc = FVector::ZeroVector;
	if(!FollowParam.FollowActor.IsValid())
	{
		return FVector::ZeroVector;
	}

	bool bSocketExist = false;
	if(FollowParam.SocketName != NAME_None)
	{
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(FollowParam.FollowActor))
		{
			if(TargetCharacter->GetMesh()->DoesSocketExist(FollowParam.SocketName))
			{
				PivotLoc = TargetCharacter->GetMesh()->GetSocketLocation(FollowParam.SocketName);
				bSocketExist = true;
			}
		}
	}
	if(!bSocketExist)
	{
		PivotLoc = FollowParam.FollowActor->GetActorLocation();
	}

	return PivotLoc;
}

FVector AGPlayerCameraManager::GetCameraComponentLocation() const
{
	FVector Loc =  GetCameraCachePOV().Location;

	UE_LOG(LogTemp, Display, TEXT("AGPlayerCameraManager::GetCameraLocation CachePOV Location:%s"), *Loc.ToString());
	if(ActiveCamera.IsValid())
	{
		Loc = ActiveCamera->GetCameraComponent()->GetComponentLocation();
		UE_LOG(LogTemp, Display, TEXT("AGPlayerCameraManager::GetCameraLocation CameraComponent Location:%s"), *Loc.ToString());
	}
	return Loc;
}

void AGPlayerCameraManager::SetConfig(int32 CfgId)
{
	if(!UGConfigDataAsset::Get()->CameraConfigs.Contains(CfgId))
	{
		UE_LOG(LogTemp, Error, TEXT("AGPlayerCameraManager::SetConfig CfgId:%d"), CfgId);
		return;
	}
	Config = UGConfigDataAsset::Get()->CameraConfigs[CfgId];
	CameraCfgId = CfgId;
}

int32 AGPlayerCameraManager::GetConfig() const
{
	return CameraCfgId;
}

void AGPlayerCameraManager::CacheParam(bool bForce)
{
	if(CachedParam.IsValid && bForce == false)
	{
		return;
	}
	CachedParam.IsValid = true;
	CachedParam.CameraParam = TargetParam;
	CachedParam.FollowParam = FollowParam;
}

void AGPlayerCameraManager::ApplyCacheParam(bool bImmediately)
{
	if(CachedParam.IsValid == false)
	{
		return;
	}
	CachedParam.IsValid = false;
	TargetParam = CachedParam.CameraParam;
	SetFollow(CachedParam.FollowParam.FollowActor.Get(), CachedParam.FollowParam.SocketName);
	if(bImmediately)
	{
		CurrentParam = TargetParam;
		RefreshParam();
	}
	ParamDirty = !bImmediately;
}

void AGPlayerCameraManager::ClearCacheParam()
{
	CachedParam.IsValid = false;
}

void AGPlayerCameraManager::SetCameraParam(const FGCameraParam& Param, bool bImmediately)
{
	SetRoll(Param.Roll, bImmediately);
	SetPitch(Param.Pitch);
	SetYaw(Param.Yaw);
	SetDistance(Param.Distance);
	if (bImmediately)
	{
		Immediately();
	}
}

void AGPlayerCameraManager::SetRoll(float Roll, bool bImmediately)
{
	CullDeg(Roll);
	TargetParam.Roll = Roll;
	if(bImmediately)
	{
		CurrentParam.Roll = TargetParam.Roll;
		RefreshParam();
	}
	ParamDirty = true;
}

void AGPlayerCameraManager::SetPitch(float Pitch, bool bImmediately)
{
	CullDeg(Pitch);
	TargetParam.Pitch = FMath::Clamp(Pitch, Config.MinPitch, Config.MaxPitch);
	if(bImmediately)
	{
		CurrentParam.Pitch = TargetParam.Pitch;
		RefreshParam();
	}
	ParamDirty = true;
}

void AGPlayerCameraManager::SetPitchDelta(float Delta, bool bImmediately)
{
	SetPitch(TargetParam.Pitch + Delta, bImmediately);
}

void AGPlayerCameraManager::SetYawDelta(float Delta, bool bImmediately)
{
	SetYaw(TargetParam.Yaw + Delta, bImmediately);
}

void AGPlayerCameraManager::SetDistanceDelta(float Delta, bool bImmediately)
{
	SetDistance(TargetParam.Distance + Delta, bImmediately);
}

void AGPlayerCameraManager::SetYaw(float Yaw, bool bImmediately)
{
	CullDeg(Yaw);
	TargetParam.Yaw =  Yaw;
	if(bImmediately)
	{
		CurrentParam.Yaw = TargetParam.Yaw;
		RefreshParam();
	}
	ParamDirty = true;
}

void AGPlayerCameraManager::SetDistance(float Distance, bool bImmediately)
{
	TargetParam.Distance = FMath::Clamp(Distance, Config.MinFollowDistance, Config.MaxFollowDistance);
	if(bImmediately)
	{
		CurrentParam.Distance = TargetParam.Distance;
	}
	ParamDirty = true;
}

float AGPlayerCameraManager::GetPitch(bool bImmediately)
{
	return bImmediately ? CurrentParam.Pitch : TargetParam.Pitch;
}

float AGPlayerCameraManager::GetYaw(bool bImmediately)
{
	return bImmediately ? CurrentParam.Yaw : TargetParam.Yaw;
}

float AGPlayerCameraManager::GetDistance(bool bImmediately)
{
	return bImmediately ? CurrentParam.Distance : TargetParam.Distance;
}

void AGPlayerCameraManager::Immediately()
{
	TickParam(0.f);
}

void AGPlayerCameraManager::TickFollow(float DeltaSeconds)
{
	if(!FollowParam.FollowActor.IsValid() || !ActiveCamera.IsValid())
	{
		return;
	}

	FVector TargetLoc = GetPivotLocation();
	const FVector CameraLoc = ActiveCamera->GetActorLocation();
	float Distance = FVector::Distance(TargetLoc, CameraLoc);
	if (Distance > Config.MinFollowSpeed * DeltaSeconds)
	{
		float Alpha = (Distance - Config.MinFollowDistance) / (Config.MaxFollowDistance - Config.MinFollowDistance);
		float FollowSpeed = FMath::Lerp(Config.MinFollowSpeed, Config.MaxFollowSpeed, Alpha);
		
		const float MoveDistance = FollowSpeed * DeltaSeconds;
		FVector Direction = (TargetLoc - CameraLoc);
		Direction.Normalize();
		if (Distance >= MoveDistance)
		{
			TargetLoc = CameraLoc + Direction * MoveDistance;
		}
	}
	ActiveCamera->SetActorLocation(TargetLoc);

}

void AGPlayerCameraManager::TickParam(float DeltaSeconds)
{
	if(!ParamDirty)
	{
		return;
	}
	
	SetDistance(TargetParam.Distance, false);
	SetYaw(TargetParam.Yaw, false);
	SetPitch(TargetParam.Pitch, false);
	SetRoll(TargetParam.Roll, false);
	
	ParamDirty = false;
	
	if (!LerpParameter(CurrentParam.Yaw, TargetParam.Yaw, Config.RotateSpeed, DeltaSeconds))
	{
		ParamDirty = true;
	}

	if (!LerpParameter(CurrentParam.Pitch, TargetParam.Pitch, Config.RotateSpeed, DeltaSeconds))
	{
		ParamDirty = true;
	}

	if (!LerpParameter(CurrentParam.Distance, TargetParam.Distance, Config.ZoomSpeed, DeltaSeconds))
	{
		ParamDirty = true;
	}

	RefreshParam();
}

void AGPlayerCameraManager::RefreshParam()
{
	if(!ActiveCamera.IsValid())
	{
		return;
	}
	
	FTransform Transform;

	//朝向
	const FRotator Rotator(CurrentParam.Pitch, CurrentParam.Yaw, CurrentParam.Roll);
	Transform.SetRotation( FQuat(Rotator) );

	//FRotationMatrix BaseRayMatrix(Rotator);
	//FVector BaseRayLocalUp, BaseRayLocalFwd, BaseRayLocalRight;
	//BaseRayMatrix.GetScaledAxes(BaseRayLocalFwd, BaseRayLocalRight, BaseRayLocalUp);
	

	//位移
	{
		//根据距离，计算相对偏移位置
		float Length = Config.MaxZoomDistance - Config.MinZoomDistance;
		float t = Length > 0.0f? t = FMath::Clamp((CurrentParam.Distance - Config.MinZoomDistance) / Length, 0.0f, 1.0f) : 0.0f;
		FVector DeltaPos = Config.DeltaNearPos + (Config.DeltaFarPos - Config.DeltaNearPos) * t ;
		
		const FVector& BackVector = Transform.GetRotation().GetForwardVector() * -1;
		const FVector& RightVector = Transform.GetRotation().GetRightVector();
		const FVector& UpVector = Transform.GetRotation().GetUpVector();
		
		FVector RelativeLocation = BackVector * (CurrentParam.Distance + DeltaPos.X)  + DeltaPos.Y * RightVector + DeltaPos.Z * UpVector;

		//PreventCameraPenetration
		{
			const FVector TargetLoc = ActiveCamera->GetActorLocation();
			const FVector CameraLoc = TargetLoc + RelativeLocation; //ActiveCamera->GetCameraComponent()->GetComponentLocation();
			
			FCollisionQueryParams SphereParams(SCENE_QUERY_STAT(CameraPen), false, nullptr/*PlayerCamera*/);
			SphereParams.AddIgnoredActor(FollowParam.FollowActor.Get());
			FCollisionShape SphereShape = FCollisionShape::MakeSphere(0.f);
			UWorld* World = GetWorld();
			SphereShape.Sphere.Radius = 5;//测试数据
			ECollisionChannel TraceChannel = ECC_Camera;
			FHitResult Hit;
			const bool bHit = World->SweepSingleByChannel(Hit, TargetLoc, CameraLoc, FQuat::Identity, TraceChannel, SphereShape, SphereParams);
			const AActor* HitActor = Hit.GetActor();
			if(bHit && HitActor)
			{
				UE_LOG(LogTemp, Display, TEXT("TickFollow HitActor=%s"), *HitActor->GetName());
				RelativeLocation = Hit.Location - TargetLoc;
			}

			//UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TargetLoc, CameraLoc, 5.f, UEngineTypes::ConvertToTraceType(ECC_Camera), true, {FollowParam.FollowActor.Get()}, EDrawDebugTrace::ForDuration, Hit, true);
		}
		Transform.SetLocation(RelativeLocation);
	}

	if(ActiveCamera.IsValid())
	{
		ActiveCamera->GetCameraComponent()->SetRelativeTransform(Transform);
	}
}

bool AGPlayerCameraManager::LerpParameter(float& Current, float Target, float Speed, float DeltaTime)
{
	float Difference = FMath::Abs(Target - Current);
	bool Arrive = false;
	float Step = Speed * DeltaTime;
	if (Step > Difference)
	{
		Step = Difference;
		Arrive = true;
	}
	if (Current < Target)
	{
		Current += Step;
	}
	else
	{
		Current -= Step;
	}
	return Arrive;
}
