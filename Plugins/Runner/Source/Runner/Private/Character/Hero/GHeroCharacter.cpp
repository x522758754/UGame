// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/GHeroCharacter.h"

#include "Pixel2DTDComponent.h"
#include "Player/GPlayerState.h"
#include "AbilitySystem/GAbilityDefine.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Character/Hero/GHeroInfoComponent.h"

AGHeroCharacter::AGHeroCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UGHeroInfoComponent>(AGCharacter::InfoComponentName))
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

void AGHeroCharacter::Move(FVector Direction, float Scale)
{
	if(Direction.IsNearlyZero())
	{
		StopMove();
		return;
	}
	
	AddMovementInput(Direction, Scale);
	Direction.Normalize();

	if(FMath::Abs(Direction.X) > FMath::Abs(Direction.Y))
	{
		if(Direction.X > 0)
		{
			MoveDirection = EGMoveDirection::Right;
		}
		else if(Direction.X < 0)
		{
			MoveDirection = EGMoveDirection::Left;
		}
	}
	else
	{
		if(Direction.Y > 0)
		{
			MoveDirection = EGMoveDirection::Down;
		}
		else if(Direction.Y < 0)
		{
			MoveDirection = EGMoveDirection::UP;
		}
	}
	isMoving = true;
	OnMoveUpdate();
}

void AGHeroCharacter::StopMove()
{
	isMoving = false;
}

FVector AGHeroCharacter::GetMoveDirection()
{
	switch (MoveDirection)
	{
		case EGMoveDirection::Right:
			return FVector(1.f, 0.f, 0.f);
		case EGMoveDirection::Left:
			return FVector(-1.f, 0.f, 0.f);
		case EGMoveDirection::Down:
			return FVector(0.f, 1.f, 0.f);
		case EGMoveDirection::UP:
			return FVector(0.f, -1.f, 0.f);
	}
	return FVector::ZeroVector;
}

void AGHeroCharacter::OnMoveUpdate()
{
	if(MoveDirection == EGMoveDirection::Right)
	{
		GetSprite()->SetWorldRotation(FRotator(0.f, 0.f, -90.f));
	}
	else if(MoveDirection == EGMoveDirection::Left)
	{
		GetSprite()->SetWorldRotation(FRotator(0.f, 180.f, 90.f));
	}
}

void AGHeroCharacter::BindAscInput()
{
	if(AbilitySystemComponent.IsValid() && InputComponent)
	{
		FGameplayAbilityInputBinds InputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"),
			FString("EGAbilityInputID"),
			static_cast<int32>(EGAbilityInputID::Confirm),
			static_cast<int32>(EGAbilityInputID::Cancel));
		
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, MoveTemp(InputBinds));
	}
}
