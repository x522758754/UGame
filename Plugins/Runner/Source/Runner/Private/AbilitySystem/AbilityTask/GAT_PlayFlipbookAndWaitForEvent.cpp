// Copyright 2020 Dan Kestranek.


#include "AbilitySystem/AbilityTask/GAT_PlayFlipbookAndWaitForEvent.h"

#include "Pixel2DTDComponent.h"
#include "Character/GCharacterBase.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "PaperFlipbook.h"
#include "AbilitySystemGlobals.h"
#include "Character/Hero/GHeroCharacter.h"

UGAT_PlayFlipbookAndWaitForEvent::UGAT_PlayFlipbookAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

UGAT_PlayFlipbookAndWaitForEvent* UGAT_PlayFlipbookAndWaitForEvent::PlayFlipbookAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UPaperFlipbook* FilpbookToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
	UGAT_PlayFlipbookAndWaitForEvent* MyObj = NewAbilityTask<UGAT_PlayFlipbookAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->FlipbookToPlay = FilpbookToPlay;
	MyObj->EventTags = EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return MyObj;
}

void UGAT_PlayFlipbookAndWaitForEvent::Activate()
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
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGAT_PlayFlipbookAndWaitForEvent::OnGameplayEvent));
			Hero->AttackType = EGAttackType::RangedAttack;
			Hero->MoveDirection = EGMoveDirection::Down;
			Hero->isAttacking = true;
			if (ShouldBroadcastAbilityTaskDelegates() == false)
			{
				return;
			}

			CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UGAT_PlayFlipbookAndWaitForEvent::OnAbilityCancelled);
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
		UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(FlipbookToPlay), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			//ABILITY_LOG(Display, TEXT("%s: OnCancelled"), *GetName());
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
	
}

void UGAT_PlayFlipbookAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();
}

FString UGAT_PlayFlipbookAndWaitForEvent::GetDebugString() const
{
	UPaperFlipbook* PlayingFlipbook = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		AGCharacterBase* CharacterBase = Cast<AGCharacterBase>(ActorInfo->AvatarActor.Get());
		if(CharacterBase)
		{
			PlayingFlipbook = CharacterBase->GetSprite()->GetFlipbook(); 
		}
	}
	
	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(FlipbookToPlay), *GetNameSafe(PlayingFlipbook));
}

void UGAT_PlayFlipbookAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingFlipbook();
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

bool UGAT_PlayFlipbookAndWaitForEvent::StopPlayingFlipbook()
{
	return true;
}

void UGAT_PlayFlipbookAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingFlipbook())
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UGAT_PlayFlipbookAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}
