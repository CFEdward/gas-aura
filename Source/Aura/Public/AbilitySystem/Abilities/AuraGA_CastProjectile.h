// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraGA_CastProjectile.generated.h"

class AAuraProjectile;

UCLASS()
class AURA_API UAuraGA_CastProjectile : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	int32 GetProjectileCount(const float InLevel = 1.f) const;
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch = false, const float PitchOverride = 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat NumProjectiles;
};
