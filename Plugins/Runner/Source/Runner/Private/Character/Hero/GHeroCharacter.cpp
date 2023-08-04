// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroCharacter.h"

#include "Player/GPlayerState.h"
#include "AbilitySystem/GAbilityDefine.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Character/Hero/GHeroInfoComponent.h"

AGHeroCharacter::AGHeroCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UGHeroInfoComponent>(AGCharacterBase::InfoComponentName))
{
}

void AGHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAscInput();
}

void AGHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AGHeroCharacter::BindAscInput()
{
	if(AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FGameplayAbilityInputBinds InputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"),
			FString("EGAbilityInputID"),
			static_cast<int32>(EGAbilityInputID::Confirm),
			static_cast<int32>(EGAbilityInputID::Cancel));
		
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, MoveTemp(InputBinds));
	}
}
