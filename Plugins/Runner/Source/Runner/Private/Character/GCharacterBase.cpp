// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GCharacterBase.h"

#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/GAbilityDefine.h"
#include "AbilitySystem/GGameplayAbility.h"
#include "AbilitySystem/Attributes/GAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "Character/Component/GCharacterMovementComponent.h"
#include "Character/GCharacterInfoComponent.h"

FName AGCharacterBase::InfoComponentName(TEXT("InfoComponent"));

// Sets default values
AGCharacterBase::AGCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	InfoComponent = CreateDefaultSubobject<UGCharacterInfoComponent>(InfoComponentName);

	bAlwaysRelevant = true;
}

UAbilitySystemComponent* AGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AGCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AGCharacterBase::GetAbilityLevel(EGAbilityInputID AbilityID) const
{
	return 1;
}

void AGCharacterBase::RemoveCharacterAbilities()
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

EGDHitReactDirection AGCharacterBase::GetHitReactDirection(const FVector & ImpactPoint)
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

void AGCharacterBase::PlayHitReact_Implementation(FGameplayTag HitDirection, AActor * DamageCauser)
{
	if (IsAlive())
	{
		if (HitDirection == FGGameplayTags::Get().Effect_HitReact_Left )
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Left);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReact_Front)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Front);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReact_Right)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Right);
		}
		else if (HitDirection == FGGameplayTags::Get().Effect_HitReact_Back )
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Back);
		}
	}
}

bool AGCharacterBase::PlayHitReact_Validate(FGameplayTag HitDirection, AActor * DamageCauser)
{
	return true;
}

int32 AGCharacterBase::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0;
}

float AGCharacterBase::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float AGCharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float AGCharacterBase::GetMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMana();
	}

	return 0.0f;
}

float AGCharacterBase::GetMaxMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxMana();
	}

	return 0.0f;
}

float AGCharacterBase::GetStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float AGCharacterBase::GetMaxStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float AGCharacterBase::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

float AGCharacterBase::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}

	return 0.0f;
}

// Run on Server and all clients
void AGCharacterBase::Die()
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

void AGCharacterBase::FinishDying()
{
	Destroy();
}

// Called when the game starts or when spawned
void AGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGCharacterBase::AddCharacterAbilities()
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

void AGCharacterBase::InitializeAttributes()
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

void AGCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void AGCharacterBase::SetMana(float Mana)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMana(Mana);
	}
}

void AGCharacterBase::SetStamina(float Stamina)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}

