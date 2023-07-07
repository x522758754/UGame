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

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);
public:
	FGameplayTag Effect_HitReact_Front;
	FGameplayTag Effect_HitReact_Back;
	FGameplayTag Effect_HitReact_Right;
	FGameplayTag Effect_HitReact_Left;

	FGameplayTag State_Dead;
	FGameplayTag Effect_RemoveOnDeath;
protected:
	void AddAllTags();
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
private:

	static FGGameplayTags GameplayTags;
};
