// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraGA_CastBeam.generated.h"

UCLASS()
class AURA_API UAuraGA_CastBeam : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UAuraGA_CastBeam();

	UFUNCTION(BlueprintCallable)
	bool StoreMouseDataInfo(const FHitResult& HitResult);
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
	UFUNCTION(BlueprintCallable)
	void RemoveOnDeathBindingFromPrimaryTarget();
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
	UFUNCTION(BlueprintCallable)
	void RemoveOnDeathBindingFromAdditionalTarget(AActor* AdditionalActor);

	//int32 GetMaxNumShockTargets() const { return MaxNumShockTargets; }
	
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	//UPROPERTY(EditDefaultsOnly, Category = "Beam")
	//int32 MaxNumShockTargets = 5;
};
