// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadSlot::InitializeSlot() const
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UVM_LoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UVM_LoadSlot::SetPlayerName(const FText& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UVM_LoadSlot::SetSelectButtonIsEnabled(const bool& bInSelectButtonIsEnabled)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSelectButtonIsEnabled, bInSelectButtonIsEnabled);
}
