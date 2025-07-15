// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadSlot::InitializeSlot() const
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}
