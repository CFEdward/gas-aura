// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AuraGA_CastProjectile.h"
#include "AuraGA_FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGA_FireBolt : public UAuraGA_CastProjectile
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch, float PitchOverride);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;
};
