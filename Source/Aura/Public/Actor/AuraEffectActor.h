// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

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
	
protected:

	//~ Begin AActor override
	virtual void BeginPlay() override;
	//~ End AActor override
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InstantEffectApplicationPolicy;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> DurationEffectApplicationPolicy;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InfiniteEffectApplicationPolicy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectRemovalPolicy> InfiniteEffectRemovalPolicy;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel;

};
