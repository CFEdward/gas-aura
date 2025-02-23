// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns")
	FScalableFloat CooldownDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldowns")
	FGameplayTagContainer CooldownTags;
	/**
	 * Temporary container that we will return the pointer to in GetCooldownTags()
	 * This will be a union of our CooldownTags and the Cooldown GE's cooldown tags
	 */
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
};
