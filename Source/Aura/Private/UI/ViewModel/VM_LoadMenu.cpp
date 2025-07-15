// Copyright Eduard Ciofu


#include "UI/ViewModel/VM_LoadMenu.h"

#include "Game/AuraGameModeBase.h"
#include "Game/LoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/VM_LoadSlot.h"

void UVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UVM_LoadSlot>(this, LoadSlotVMClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

void UVM_LoadMenu::NewSlotButtonPressed(const int32 Slot)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SlotStatus = Taken;
	
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UVM_LoadMenu::NewGameButtonPressed(const int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
	LoadSlots[Slot]->SetPlayerName(FText());
}

void UVM_LoadMenu::SelectSlotButtonPressed(const int32 Slot)
{
	for (const TTuple<int32, UVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->SetSelectButtonIsEnabled(LoadSlot.Key != Slot);
	}
	SelectedSlot = LoadSlots[Slot];
	SetPlayButtonIsEnabled(true);
	SetDeleteButtonIsEnabled(true);
}

void UVM_LoadMenu::LoadData()
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		const ULoadMenuSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		
		LoadSlot.Value->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(FText::FromString(SaveObject->PlayerName));
		LoadSlot.Value->InitializeSlot();
	}
}

void UVM_LoadMenu::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->SetSelectButtonIsEnabled(true);
		SetDeleteButtonIsEnabled(false);
		SetPlayButtonIsEnabled(false);
		SelectedSlot->InitializeSlot();
	}
}

UVM_LoadSlot* UVM_LoadMenu::GetLoadSlotVMByIndex(const int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}
