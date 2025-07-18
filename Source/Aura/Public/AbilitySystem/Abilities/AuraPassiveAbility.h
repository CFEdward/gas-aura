﻿// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:

	UFUNCTION()
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
