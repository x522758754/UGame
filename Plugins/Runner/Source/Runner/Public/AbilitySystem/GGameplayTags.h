#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameplayTagContainer.h"
#include "HAL/Platform.h"

struct FGGameplayTags
{
public:

	static const FGGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	//获取GameplayTag引用
	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);
public:
	FGameplayTag Effect_HitReactFront;
	FGameplayTag Effect_HitReactBack;
	FGameplayTag Effect_HitReactRight;
	FGameplayTag Effect_HitReactLeft;
	
	FGameplayTag Effect_RemoveOnDeath;
	
	FGameplayTag State_Dead;
	FGameplayTag State_AimDownSights;
	FGameplayTag State_AimDownSightsRemoval;
	

	FGameplayTag Event_MontageEndAbility;
	FGameplayTag Event_MontageSpawnProjectile;
protected:
	void AddAllTags();
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
private:

	static FGGameplayTags GameplayTags;
};
