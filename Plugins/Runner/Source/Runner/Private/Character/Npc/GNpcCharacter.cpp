// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Npc/GNpcCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AbilitySystem/GAbilitySystemComponent.h"
#include "AbilitySystem/GGameplayTags.h"
#include "AbilitySystem/Attributes/GAttributeSetBase.h"

#include "Character/AI/GAIItem.h"
#include "Character/Npc/GNpcInfoComponent.h"

#include "Player/GNpcAIController.h"

#include "System/GAssetManager.h"

AGNpcCharacter::AGNpcCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGNpcInfoComponent>(AGCharacter::InfoComponentName))
{
	HardRefAbilitySystemComponent = CreateDefaultSubobject<UGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSetBase = CreateDefaultSubobject<UGAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set our parent's TWeakObjectPtr
	AttributeSetBase = HardRefAttributeSetBase;

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AGNpcCharacter::RefreshNpcConfig()
{
#if WITH_EDITOR
	FGNpcConfig& NpcConfig = GetInfoComponent<UGNpcInfoComponent>()->GetMutableNpcConfig();
	NpcConfig.Transform = GetActorTransform();
	NpcConfig.NpcClass = GetClass();
#endif
}

void AGNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attribute change callbacks
	HealthChangedDelegateHandle = AbilitySystemComponent->
	                              GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).
	                              AddUObject(this, &AGNpcCharacter::HealthChanged);

	// Tag change callbacks
	AbilitySystemComponent->RegisterGameplayTagEvent(FGGameplayTags::Get().State_DebuffStun,
	                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &AGNpcCharacter::StunTagChanged);

	GetInfoComponent<UGNpcInfoComponent>()->OnNpcConfigApplied.BindUObject(this, &AGNpcCharacter::ApplyNpcConfig);
}

void AGNpcCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// If the minion died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(FGGameplayTags::Get().State_Dead))
	{
		Die();
	}
}

void AGNpcCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGGameplayTags::Get().Ability);

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGGameplayTags::Get().Ability_NotCanceledByStun);

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void AGNpcCharacter::ApplyNpcConfig(const FGNpcConfig& InNpcConfig)
{
	if(!InNpcConfig.AIItem)
	{
		return;
	}
	SpawnController();

	//应用行为树
	if (AGNpcAIController* AIContoller = Cast<AGNpcAIController>(Controller))
	{
		AIContoller->NpcAIItem = InNpcConfig.AIItem;
		if(UBehaviorTree* BehaviorTree = UGAssetManager::LoadAsset(InNpcConfig.AIItem->BehaviorTree))
		{
			AIContoller->RunBehaviorTree(BehaviorTree);
		}
	}
}

void AGNpcCharacter::SpawnController()
{
	if (Controller != nullptr || !HasAuthority() || AIControllerClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetLevel();
	SpawnInfo.ObjectFlags |= RF_Transient; // We never want to save AI controllers into a map
	AController* NewController = GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(),
	                                                                 GetActorRotation(), SpawnInfo);
	if (NewController != nullptr)
	{
		// if successful will result in setting this->Controller 
		// as part of possession mechanics
		NewController->Possess(this);
	}
}
