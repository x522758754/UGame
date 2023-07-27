// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Pixel2DTDCharacter.h"
#include "GCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EGDHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGCharacterBase*, Character);

class UGCharacterInfoComponent;
/**
 * 
 */
UCLASS()
class RUNNER_API AGCharacterBase : public APixel2DTDCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGCharacterBase(const FObjectInitializer& ObjectInitializer);

	// Set the Hit React direction in the Animation Blueprint
	UPROPERTY(BlueprintAssignable)
	FCharacterBaseHitReactDelegate ShowHitReact;

	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDied;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable)
	virtual int32 GetAbilityLevel(EGAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable)
	EGDHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual void PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual bool PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser);

	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	
	UFUNCTION(BlueprintCallable)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMana() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeedBaseValue() const;


	virtual void Die();

	UFUNCTION(BlueprintCallable)
	virtual void FinishDying();
	
public:
	static FName InfoComponentName;
	
	template <class T>
	T* GetInfoComponent() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, UGCharacterInfoComponent>::Value, "'T' template parameter to GetInfoComponent must be derived from UGCharacterInfoComponent");
		return Cast<T>(InfoComponent);
	}

protected:

	virtual void BeginPlay() override;
	
	virtual void AddCharacterAbilities();
	
	virtual void InitializeAttributes();
	
	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetStamina(float Stamina);

public:

protected:

	TWeakObjectPtr<class UGAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UGAttributeSetBase> AttributeSetBase;
	


	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<class UGGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UGCharacterInfoComponent* InfoComponent;
};
