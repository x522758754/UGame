// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GGameplayAbility.h"
// #include "Characters/Abilities/AbilityTasks/GDAT_PlayMontageAndWaitForEvent.h"
// #include "Characters/GDProjectile.h"
#include "GGA_Archery.generated.h"

class UPaperFlipbook;
class AGProjectile;
/**
 * 
 */
UCLASS()
class UGGA_Archery : public UGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGGA_Archery();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPaperFlipbook* FireFlipbook;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AGProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag TagEvent_MontageSpawnProjectile;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag TagEvent_MontageEndAbility;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
