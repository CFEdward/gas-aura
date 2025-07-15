// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadSlot::InitializeSlot()
{
	// TODO: Check slot status based on loaded data
	SetWidgetSwitcherIndex.Broadcast(2);
}

void UVM_LoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UVM_LoadSlot::SetPlayerName(const FText& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}
