// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroCharacter.h"
#include "Character/Hero/GHeroInfoComponent.h"

AGHeroCharacter::AGHeroCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UGHeroInfoComponent>(AGCharacterBase::InfoComponentName))
{
}
