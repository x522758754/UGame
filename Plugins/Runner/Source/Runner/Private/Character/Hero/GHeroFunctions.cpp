// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroFunctions.h"

#include "System/GGameInstance.h"
#include "Character/Hero/GHeroInfoComponent.h"
#include "Config/GConfigDataAsset.h"
#include "Character/Hero/GHeroCharacter.h"
#include "Character/Data/GHeroData.h"
#include "System/GAssetManager.h"
#include "System/GCommonFunctions.h"

AGHeroCharacter* UGHeroFunctions::SpawnHero(int32 ConfigId , const FTransform& Transform)
{
	if(!UGCommonFunctions::GetCurWorld())
	{
		return nullptr;
	}
	if(!UGConfigDataAsset::Get()->HeroConfigs.Contains(ConfigId))
	{
		return nullptr;
	}

	const FGHeroConfig& Cfg = UGConfigDataAsset::Get()->HeroConfigs[ConfigId];
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
	UGHeroInfoComponent* InfoComponent = Hero->GetInfoComponent<UGHeroInfoComponent>();
	if(InfoComponent)
	{
		InfoComponent->SetConfigId(ConfigId);
		InfoComponent->SetGuid(UGGameInstance::Get()->GetGuidFactory().GetNextPlayerGuid());
	}

	return Hero;
}
