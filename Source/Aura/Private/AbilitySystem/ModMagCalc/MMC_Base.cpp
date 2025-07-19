// Copyright Eduard Ciofu


#include "AbilitySystem/ModMagCalc/MMC_Base.h"

#include "Interaction/CombatInterface.h"

float UMMC_Base::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// empty for now just to pass  
	FAggregatorEvaluateParameters EvalParams;
	
	const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
 
	float AttributeContribution = 0.f;
    
	for (FAttributeCoefficient AttributeCoefficient : AttributeCoefficients)
	{
		if (const FGameplayEffectAttributeCaptureDefinition* CapturedAttribute = RelevantAttributesToCapture.FindByPredicate(
			[AttributeCoefficient](const FGameplayEffectAttributeCaptureDefinition& Item)
		   {
			  return Item.AttributeToCapture.AttributeName == AttributeCoefficient.Attribute.AttributeName;
		   }
		))
		{
			float AttValue = 0.f;
			GetCapturedAttributeMagnitude(*CapturedAttribute, Spec, EvalParams, AttValue);
			AttValue = FMath::Max<float>(AttValue, 0.f);
			switch (AttributeCoefficient.Operation)
			{
			case EGameplayModOp::Additive:
				AttributeContribution += (AttributeCoefficient.Coefficient + AttValue);
				break;
				
			case EGameplayModOp::Multiplicitive:
				AttributeContribution += (AttributeCoefficient.Coefficient * AttValue);
				break;
				
			case EGameplayModOp::Division:
				AttributeContribution += (AttributeCoefficient.Coefficient / AttValue);
				break;
				
			default: break;
			}
		}
	}
 
	// BaseValue + Coeff1 * Att1 ... + LevelMult * PlayerLevel
	return BaseValue + AttributeContribution + LevelMultiplier * PlayerLevel;
}