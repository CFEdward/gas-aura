// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "VM_LoadMenu.generated.h"

class UVM_LoadSlot;

UCLASS()
class AURA_API UVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(const int32 Slot);
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const int32 Slot);
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(const int32 Slot);

	void LoadData();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_LoadSlot> LoadSlotVMClass;
	
	UFUNCTION(BlueprintPure)
	UVM_LoadSlot* GetLoadSlotVMByIndex(const int32 Index) const;

	/** Field Notifies */
	
	void SetPlayButtonIsEnabled(const bool& bInIsEnabled) { UE_MVVM_SET_PROPERTY_VALUE(bPlayButtonIsEnabled, bInIsEnabled); }
	void SetDeleteButtonIsEnabled(const bool& bInIsEnabled) { UE_MVVM_SET_PROPERTY_VALUE(bDeleteButtonIsEnabled, bInIsEnabled); }

	bool GetPlayButtonIsEnabled() const { return bPlayButtonIsEnabled; }
	bool GetDeleteButtonIsEnabled() const { return bDeleteButtonIsEnabled; }

private:

	UPROPERTY()
	TMap<int32, TObjectPtr<UVM_LoadSlot>> LoadSlots;
	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_2;

	/** Field Notifies */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter = SetPlayButtonIsEnabled, Getter = GetPlayButtonIsEnabled, meta = (AllowPrivateAccess = "true"))
	bool bPlayButtonIsEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter = SetDeleteButtonIsEnabled, Getter = GetDeleteButtonIsEnabled, meta = (AllowPrivateAccess = "true"))
	bool bDeleteButtonIsEnabled = false;
};
