// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/AbilityTask/GAT_AttackAndWaitForEvent.h"

#include "Pixel2DTDComponent.h"
#include "Character/GCharacter.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "PaperFlipbook.h"
#include "AbilitySystemGlobals.h"
#include "Character/Hero/GHeroCharacter.h"

UGAT_AttackAndWaitForEvent::UGAT_AttackAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
{
}

UGAT_AttackAndWaitForEvent* UGAT_AttackAndWaitForEvent::AttackAndWaitForEvent(UGameplayAbility* OwningAbility, EGAttackType Type, FName TaskInstanceName, FGameplayTagContainer EventTags)
{
	UGAT_AttackAndWaitForEvent* MyObj = NewAbilityTask<UGAT_AttackAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->EventTags = EventTags;
	MyObj->AttackType = Type;

	return MyObj;
}

void UGAT_AttackAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}
	bool bPlayed = false;
	if(AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		AGHeroCharacter* Hero = Cast<AGHeroCharacter>(GetAvatarActor());
		if(Hero)
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGAT_AttackAndWaitForEvent::OnGameplayEvent));
			Hero->isAttacking = true;
			Hero->AttackType = AttackType;
			Hero->GetSprite()->OnFinishedPlaying.AddDynamic(this, &UGAT_AttackAndWaitForEvent::OnFinishedPlaying);
			if (ShouldBroadcastAbilityTaskDelegates() == false)
			{
				return;
			}

			CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UGAT_AttackAndWaitForEvent::OnAbilityCancelled);
			bPlayed = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
	}

	if (!bPlayed)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed; Task Instance Name %s."), *Ability->GetName(), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			//ABILITY_LOG(Display, TEXT("%s: OnCancelled"), *GetName());
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UGAT_AttackAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();
}

void UGAT_AttackAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if(AbilityEnded)
		{
			StopPlaying();
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UGAT_AttackAndWaitForEvent::OnFinishedPlaying()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
	}

	EndTask();
}

bool UGAT_AttackAndWaitForEvent::StopPlaying()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	AGHeroCharacter* Hero = Cast<AGHeroCharacter>(GetAvatarActor());
	if(!Hero)
	{
		return false;
	}

	if (AbilitySystemComponent && Ability)
	{
		if (/*AbilitySystemComponent->GetAnimatingAbility() == Ability && */Hero->AttackType == AttackType)
		{
			Hero->isAttacking = false;
			Hero->AttackType = EGAttackType::None;
			Hero->GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &UGAT_AttackAndWaitForEvent::OnFinishedPlaying);
			return true;
		}
	}
	
	return false;
}

void UGAT_AttackAndWaitForEvent::OnAbilityCancelled()
{
	if(StopPlaying())
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UGAT_AttackAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}
