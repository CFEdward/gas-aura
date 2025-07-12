// Copyright Eduard Ciofu


#include "AbilitySystem/Data/AbilityInfo.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGA_CastBeam.h"
#include "AbilitySystem/Abilities/AuraGA_CastProjectile.h"
#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	
	return FAuraAbilityInfo();
}

FString UAbilityInfo::FormatTextValues(const FGameplayTag& AbilityTag, const int32 Level, const FString& Description) const
{
	UAuraGameplayAbility* AbilityDefault = Cast<UAuraGameplayAbility>(FindAbilityInfoForTag(AbilityTag).Ability.GetDefaultObject());
	FStringFormatNamedArguments NamesToValues;
	
	NamesToValues.Add(TEXT("_Level"), FStringFormatArg(Level));

	if (AbilityDefault)
	{
		NamesToValues.Add(TEXT("_Cost"), FStringFormatArg(FString::SanitizeFloat(AbilityDefault->GetManaCost(Level))));
		NamesToValues.Add(TEXT("_CD"), FStringFormatArg(FString::SanitizeFloat(AbilityDefault->GetCooldown(Level))));

		if (const UAuraDamageGameplayAbility* AuraDamageGA = static_cast<UAuraDamageGameplayAbility*>(AbilityDefault))
		{
			const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
			
			NamesToValues.Add(TEXT("_FireDmg"), FStringFormatArg(FMath::RoundToInt32(AuraDamageGA->GetDamageAtLevel(Tags.Damage_Fire, Level))));
			NamesToValues.Add(TEXT("_LightningDmg"), FStringFormatArg(FMath::RoundToInt32(AuraDamageGA->GetDamageAtLevel(Tags.Damage_Lightning, Level))));
			NamesToValues.Add(TEXT("_ArcDmg"), FStringFormatArg(FMath::RoundToInt32(AuraDamageGA->GetDamageAtLevel(Tags.Damage_Arcane, Level))));
			NamesToValues.Add(TEXT("_PhysDmg"), FStringFormatArg(FMath::RoundToInt32(AuraDamageGA->GetDamageAtLevel(Tags.Damage_Physical, Level))));
			NamesToValues.Add(TEXT("_SubSpellNum"), FStringFormatArg(FMath::Min(Level, AuraDamageGA->GetMaxNumSubSpell())));
		}

		if (const UAuraGA_CastProjectile* AuraProjectileSpell = static_cast<UAuraGA_CastProjectile*>(AbilityDefault))
		{
			NamesToValues.Add(TEXT("_ProjNum"), FStringFormatArg(AuraProjectileSpell->GetProjectileCount(Level)));
		}
		if (const UAuraGA_CastBeam* AuraBeamSpell = static_cast<UAuraGA_CastBeam*>(AbilityDefault))
		{
			NamesToValues.Add(TEXT("_BeamNum"), FStringFormatArg(FMath::Min(Level - 1, AuraBeamSpell->GetMaxNumSubSpell())));
		}
	}

	return FString::Format(*Description, NamesToValues);
}
