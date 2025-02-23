// Copyright Eduard Ciofu


#include "AbilitySystem/ModMagCalc/MMC_PlayerAbilityCooldown.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

float UMMC_PlayerAbilityCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.f;
	}

	return Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());
}
