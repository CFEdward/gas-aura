// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

class UAuraUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelected, UAuraUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSetButtonsEnabled, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);

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

	UPROPERTY(BlueprintAssignable)
	FSetButtonsEnabled SetButtonsEnabledDelegate;
	UFUNCTION(BlueprintCallable)
	void SetButtonsEnabled(const FGameplayTag& AbilityTag);

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bSpendPointsButtonEnabled, bool& bEquipButtonEnabled);
};
