// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadMenu.h"

#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlots.Add(2, LoadSlot_2);
}

void UVM_LoadMenu::NewSlotButtonPressed(const int32 Slot, const FString& EnteredName)
{
	
}

void UVM_LoadMenu::NewGameButtonPressed(const int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UVM_LoadMenu::SelectSlotButtonPressed(const int32 Slot)
{
	
}

UVM_LoadSlot* UVM_LoadMenu::GetLoadSlotVMByIndex(const int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}
