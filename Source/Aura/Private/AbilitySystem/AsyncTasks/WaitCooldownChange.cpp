// Copyright Eduard Ciofu


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid()) return nullptr;
	
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// To know when a cooldown has ended (CooldownTag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::CooldownTagChanged
	);
	// To know when a cooldown effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveGameplayEffect)
{
	const bool bIsReplicatedEffect = !SpecApplied.GetContext().GetAbilityInstance_NotReplicated();
	if (bIsReplicatedEffect) return;
	
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		const TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		// Just in case there are more than 1 CooldownTags assigned (shouldn't be)
		const float TimeRemaining = FMath::Max(TimesRemaining);

		CooldownStart.Broadcast(TimeRemaining);
	}
}

void UWaitCooldownChange::CheckRemainingCooldownTime()
{
	const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

	if (TimesRemaining.Num() > 0)
	{
		float LongestTime = TimesRemaining[0];
		for (int32 i = 0; i < TimesRemaining.Num(); i++)
		{
			if (TimesRemaining[i] > LongestTime)
			{
				LongestTime = TimesRemaining[i];
			}
		}
		CooldownStart.Broadcast(LongestTime);
	}
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);

	SetReadyToDestroy();
	//MarkAsGarbage();	// Unnecessary?
}
