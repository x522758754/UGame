// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/GGameplayTags.h"

UGGameplayAbility::UGGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	ActivationBlockedTags.AddTag(FGGameplayTags::Get().State_Dead);
	ActivationBlockedTags.AddTag(FGGameplayTags::Get().State_DebuffStun);
	
	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}