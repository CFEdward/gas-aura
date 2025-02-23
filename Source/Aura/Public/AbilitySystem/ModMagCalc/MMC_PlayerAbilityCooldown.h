// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PlayerAbilityCooldown.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_PlayerAbilityCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
