// Copyright Eduard Ciofu


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	const float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, DamageMagnitude);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAuraDamageGameplayAbility::GetDamageAtLevel(const FGameplayTag& DmgType, const float InLevel) const
{
	if (DmgType == DamageType) return Damage.GetValueAtLevel(InLevel);
	else return 0.f;
}

float UAuraDamageGameplayAbility::GetDamageAtCurrentLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	
	if (IsValid(TargetActor))
	{
		const bool bKnockback = FMath::RandRange(UE_SMALL_NUMBER, 100.f) < KnockbackChance;
		if (bKnockback) Params.KnockbackForce = GetImpulse(TargetActor) * KnockbackForceMagnitude;

		Params.DeathImpulse = GetImpulse(TargetActor) * DeathImpulseMagnitude;
	}
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = RadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	
	return Params;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

FVector UAuraDamageGameplayAbility::GetImpulse(const AActor* TargetActor, const bool bOverrideDirection, const FVector& OverrideDirection, const float PitchOverride) const
{
	FVector Direction = TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();	// default
	Direction = bOverrideDirection ? OverrideDirection : Direction;

	Direction.Z = 0.f;
	Direction.Normalize();
	const FVector ToTarget = Direction + FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(PitchOverride));

	return ToTarget;
}
