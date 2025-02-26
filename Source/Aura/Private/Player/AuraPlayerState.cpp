// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

void AAuraPlayerState::AddToXP(const int32 InXP)
{
	XP += InXP;

	const int32 NewLevel = LevelUpInfo->FindLevelForXP(XP);
	const int32 LevelUpsCount = NewLevel - Level;
	if (LevelUpsCount > 0)
	{
		// Grant attribute/spell points per level surpassed
		for (int32 SurpassedLevel = Level; SurpassedLevel < NewLevel; ++SurpassedLevel)
		{
			const int32 GrantedAttributePoints = LevelUpInfo->LevelUpInformation[SurpassedLevel].AttributePointAward;
			AddToAttributePoints(GrantedAttributePoints);
			const int32 GrantedSpellPoints = LevelUpInfo->LevelUpInformation[SurpassedLevel].SpellPointAward;
			AddToSpellPoints(GrantedSpellPoints);
		}
		AddToLevel(LevelUpsCount);
	}
	
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetXP(const int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;

	// Force a recalculation of MMC_MaxHealth and BP_MMC_MaxMana's magnitudes, which depend on Level
	OnModifierDependencyChanged.Broadcast();
	// Maximize vital attributes using the updated magnitudes
	Cast<UAuraAttributeSet>(AttributeSet)->MaximizeVitalAttributes();
	
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetLevel(const int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}
