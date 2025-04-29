// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

class UAuraUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelected, UAuraUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSetButtonsEnabled, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, Description, FString, NextLevelDescription);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FAbilitySelected AbilitySelectedDelegate;
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(UAuraUserWidget* AbilityButton) const;

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	UPROPERTY(BlueprintAssignable)
	FSetButtonsEnabled SetButtonsEnabledDelegate;
	UFUNCTION(BlueprintCallable)
	void SetButtonsEnabled(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

private:

	FSelectedAbility SelectedAbility{ FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bSpendPointsButtonEnabled, bool& bEquipButtonEnabled);
};
