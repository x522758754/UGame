// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/Ability/GGA_CharacterJump.h"
#include "AbilitySystem/GGameplayTags.h"
#include "Character/GCharacter.h"

UGGA_CharacterJump::UGGA_CharacterJump()
{
	AbilityInputID = EGAbilityInputID::Jump;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	// AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

void UGGA_CharacterJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		Character->Jump();
	}
}

bool UGGA_CharacterJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	return Character && Character->CanJump();
}

void UGGA_CharacterJump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGGA_CharacterJump::CancelAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGGA_CharacterJump::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}
