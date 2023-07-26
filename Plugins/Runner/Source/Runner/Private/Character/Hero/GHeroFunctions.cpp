// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroFunctions.h"

#include "Config/GConfigData.h"
#include "Character/Hero/GHeroCharacter.h"
#include "Character/Data/GHeroData.h"
#include "System/GAssetManager.h"
#include "System/GCommonFunctions.h"

AGHeroCharacter* UGHeroFunctions::SpawnHero(int32 HeroId , const FTransform& Transform)
{
	if(!UGCommonFunctions::GetCurWorld())
	{
		return nullptr;
	}
	if(!UGConfigData::Get()->HeroConfigs.Contains(HeroId))
	{
		return nullptr;
	}

	const FGHeroConfig& Cfg = UGConfigData::Get()->HeroConfigs[HeroId];
	TSubclassOf<AGHeroCharacter> HeroClass = UGAssetManager::LoadSubclass<AGHeroCharacter>(Cfg.HeroClass);
	if(HeroClass == nullptr)
	{
		return nullptr;
	}
	AGHeroCharacter* Hero = UGCommonFunctions::GetCurWorld()->SpawnActor<AGHeroCharacter>(HeroClass, Transform);
	if(!Hero)
	{
		return nullptr;	
	}

	return Hero;
}
