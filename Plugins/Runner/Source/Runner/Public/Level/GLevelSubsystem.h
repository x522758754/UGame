// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GLevelSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGLevelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static UGLevelSubsystem* Get();
protected:
	static TWeakObjectPtr<UGLevelSubsystem> s_Instance;
public:
	void TravelLevel(const FString& Url);

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	
};
