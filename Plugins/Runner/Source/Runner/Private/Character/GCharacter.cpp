// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GCharacter.h"

#include "Player/GPlayerState.h"
#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilityDefine.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "AbilitySystem/Attributes/GAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Character/Component/GCharacterMovementComponent.h"
#include "Character/GCharacterInfoComponent.h"

FName AGCharacter::InfoComponentName(TEXT("InfoComponent"));

// Sets default values
AGCharacter::AGCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	InfoComponent = CreateDefaultSubobject<UGCharacterInfoComponent>(InfoComponentName);
	GetCapsuleComponent()->SetHiddenInGame(false);
	bAlwaysRelevant = true;
}

void AGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AGPlayerState* PS = GetPlayerState<AGPlayerState>())
	{
		AbilitySystemComponent = Cast<UGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSetBase = PS->GetAttributeSetBase();

		InitializeAttributes();

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(FGGameplayTags::Get().State_Dead, 0);

		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		AddCharacterAbilities();
	}
	
}

void AGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(AGPlayerState* PS = GetPlayerState<AGPlayerState>())
	{
		AbilitySystemComponent = Cast<UGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

UAbilitySystemComponent* AGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AGCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AGCharacter::GetAbilityLevel(EGAbilityInputID AbilityID) const
{
	return 1;
}

void AGCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

EGDHitReactDirection AGCharacter::GetHitReactDirection(const FVector & ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EGDHitReactDirection::Front;
		}
		else
		{
			return EGDHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EGDHitReactDirection::Right;
		}
		else
		{
			return EGDHitReactDirection::Left;
		}
	}

	return EGDHitReactDirection::Front;
}

void AGCharacter::PlayHitReact_Implementation(FGameplayTag HitDirection, AActor * DamageCauser)
{
	if (IsAlive())
	{
		if (HitDirection == FGGameplayTags::Get().Effect_HitReactLeft )
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Left);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReactFront)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Front);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReactRight)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Right);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReactBack )
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Back);
		}
	}
}

bool AGCharacter::PlayHitReact_Validate(FGameplayTag HitDirection, AActor * DamageCauser)
{
	return true;
}

int32 AGCharacter::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0;
}

float AGCharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float AGCharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float AGCharacter::GetMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMana();
	}

	return 0.0f;
}

float AGCharacter::GetMaxMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxMana();
	}

	return 0.0f;
}

float AGCharacter::GetStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float AGCharacter::GetMaxStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float AGCharacter::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

float AGCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}

	return 0.0f;
}

// Run on Server and all clients
void AGCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(FGGameplayTags::Get().Effect_RemoveOnDeath);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(FGGameplayTags::Get().State_Dead);
	}

	// if (DeathMontage)
	// {
	// 	PlayAnimMontage(DeathMontage);
	// }
	// else
	{
		FinishDying();
	}
}

void AGCharacter::FinishDying()
{
	Destroy();
}

// Called when the game starts or when spawned
void AGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	
	for (TSubclassOf<UGGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AGCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AGCharacter::AddStartupEffects()
{
	if(!HasAuthority() || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void AGCharacter::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
}

void AGCharacter::SetMana(float Mana)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMana(Mana);
	}
}

void AGCharacter::SetStamina(float Stamina)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}

