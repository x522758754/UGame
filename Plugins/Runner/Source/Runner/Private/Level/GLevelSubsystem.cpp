// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelSubsystem.h"

#include "System/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Loading/GLevelLoadingOpenLevel.h"

TWeakObjectPtr<UGLevelSubsystem> UGLevelSubsystem::s_Instance =	nullptr;

bool UGLevelSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

UGLevelSubsystem* UGLevelSubsystem::Get()
{
	if(s_Instance == nullptr)
	{
		s_Instance = UGGameInstance::Get()->GetSubsystem<UGLevelSubsystem>();
	}
	return s_Instance.Get();
}

const FString& UGLevelSubsystem::GetOpenLevelName() const
{
	return OpenLevelName;
}

void UGLevelSubsystem::ChangeLevel(const FString& LevelName)
{
	BeginLoading(ELoadingType::OpenLevel, LevelName);
	OpenLevelName = LevelName;
	OpenLevelLoaded = false;
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));
}


void UGLevelSubsystem::BeginLoading(ELoadingType Type, const FString& LevelId)
{
	CurrentLoadingType = Type;
	if (Type < ELoadingType::Dynamic || Type >= ELoadingType::Num)
	{
		UE_LOG(LogTemp, Fatal, TEXT("UGLevelSubsystem::SetLoadingType Error Type, %d"), Type);
		return;
	}
	switch (Type)
	{
	case ELoadingType::OpenLevel:
		CurrentLoading = MakeShared<FGLevelLoadingOpenLevel>();
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("UGLevelSubsystem::SetLoadingType %d"), Type);
		return;
	}
	CurrentLoading->Begin(LevelId);
}

