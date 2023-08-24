// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/GProjectile.h"
#include "GameplayEffect.h"
#include "GMeteorProjectile.generated.h"

UCLASS()
class AGMeteorProjectile : public AGProjectile
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle StunEffectSpecHandle;

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(UPrimitiveComponent* OtherComponent);
	
};
