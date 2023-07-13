// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GGameMode.h"

AGGameMode::AGGameMode(const FObjectInitializer& ObjectInitializer)
{
}

void AGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
