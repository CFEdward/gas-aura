// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraGA_FireBlast.generated.h"

class AAuraFireBall;

UCLASS()
class AURA_API UAuraGA_FireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UAuraGA_FireBlast();

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
