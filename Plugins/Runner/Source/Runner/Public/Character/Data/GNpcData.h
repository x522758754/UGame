// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GNpcData.generated.h"

class AGNpcCharacter;
class UBehaviorTree;
class UGNpcInfoComponent;

UENUM(BlueprintType)
enum class EGNpcType : uint8
{
	Character = 0,
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGNpcConfig
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 ConfigId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGNpcType NpcType = EGNpcType::Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="是否场景中自动创建"))
	bool AutoCreate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="客户端Npc"))
	bool ClientOnly = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CreateOnGround = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AGNpcCharacter> NpcClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;
};

USTRUCT(BlueprintType)
struct FGClientNpcInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGNpcConfig NpcConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGNpcInfoComponent* NpcInfoComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* NpcActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Hp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bFirstSpawn = true;
	TFunction<void(const FGClientNpcInfo&)> AfterNpcSpawnFunction = nullptr;

public:
	void Spawn(UWorld* World);
};
