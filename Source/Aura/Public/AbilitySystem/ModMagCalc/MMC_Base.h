// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Base.generated.h"

USTRUCT(BlueprintType)
struct AURA_API FAttributeCoefficient
{
	GENERATED_BODY()
	
	FAttributeCoefficient(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coefficient = 1.f;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ValidEnumValues="Additive, Multiplicitive, Division") )
	TEnumAsByte<EGameplayModOp::Type> Operation = EGameplayModOp::Additive;
};

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Base : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	UPROPERTY(EditAnywhere)
	float BaseValue = 1.f;

	UPROPERTY(EditAnywhere)
	TArray<FAttributeCoefficient> AttributeCoefficients;

	UPROPERTY(EditAnywhere)
	float LevelMultiplier = 10.f;
};
