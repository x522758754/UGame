#pragma once

#include "CoreMinimal.h"
#include "CharacterDef.generated.h"

UENUM(BlueprintType)
enum class EGDirection :uint8
{
	None,
	UP,
	Down,
	Right,
	Left,
};

UENUM(BlueprintType)
enum class EGAttackType:uint8
{
	MeleeAttack UMETA(DisplayName = "Melee Attack"),
	RangedAttack UMETA(DisplayName = "Ranged Attack"),
	GrenadeAttack UMETA(DisplayName = "Grenade Attack"),
	MagicAttack UMETA(DisplayName = "Magic Attack"),
	None UMETA(DisplayName = "None"),
};
