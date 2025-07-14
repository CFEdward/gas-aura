// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadMenu.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlots.Add(2, LoadSlot_2);
}

void UVM_LoadMenu::NewSlotButtonPressed(const int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->PlayerName = EnteredName;

	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
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
