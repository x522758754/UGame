#include "AbilitySystem/GGameplayTags.h"
#include "GameplayTagsManager.h"


//UE_DEFINE_GAMEPLAY_TAG(TAG_Test_Message, "Test.Message");

FGGameplayTags FGGameplayTags::GameplayTags;

void FGGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags();

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

FGameplayTag FGGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

void FGGameplayTags::AddAllTags()
{
	//Ability
	AddTag(Ability, "Ability", "Ability");
	AddTag(Ability_NotCanceledByStun, "Ability.NotCanceledByStun", "Ability Not Canceled By Stun");

	//Effect
	AddTag(Effect_HitReactFront, "Effect.HitReact.Front", "Effect Hit React Front");
	AddTag(Effect_HitReactBack, "Effect.HitReact.Back", "Effect Hit React Back");
	AddTag(Effect_HitReactRight, "Effect.HitReact.Right", "Effect Hit React Right");
	AddTag(Effect_HitReactLeft, "Effect.HitReact.Left", "Effect Hit React Left");
	
	AddTag(Effect_RemoveOnDeath, "Effect.RemoveOnDeath", "Effect Remove On Death");

	//State
	AddTag(State_Dead, "State.Dead", "State Dead");
	AddTag(State_DebuffStun, "State.Debuff.Stun", "State Debuff Stun");

	AddTag(State_AimDownSights, "State.AimDownSights", "State Aim Down Sights");
	AddTag(State_AimDownSightsRemoval, "State.AimDownSights.Removal", "State Aim Down Sights Removal");

	//Event
	AddTag(Event_MontageEndAbility, "Event.Montage.EndAbility", "Event Montage End Ability");
	AddTag(Event_MontageSpawnProjectile, "Event.Montage.SpawnProjectile", "Event Montage Spawn Projectile");

	//Data
	AddTag(Data_Damage, "Data.Damage", "Data Damage");
}

void FGGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
