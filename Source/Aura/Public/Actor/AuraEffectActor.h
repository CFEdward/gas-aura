// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class URotatingMovementComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraEffectActor();

	virtual void Tick(float DeltaTime) override;
	
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//bool bDestroyOnEffectApplication;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectRemovalPolicy> InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;

private:

	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();
	void ItemMovement(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector InitialLocation;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector CalculatedLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SineAmplitude = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SinePeriodConstant = 1.f;

	float RunningTime = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URotatingMovementComponent> RotatingMoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSinusoidalMovement = false;
};
