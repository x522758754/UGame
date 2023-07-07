// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GLevelSubsystem.h"

#include "System/GGameInstance.h"
#include "Kismet/GameplayStatics.h"

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

void UGLevelSubsystem::TravelLevel(const FString& Url)
{
	//GetWorld()->ServerTravel(Url);
	UGameplayStatics::OpenLevel(GetWorld(), FName(Url));
}

