// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_LoadSlot.generated.h"

enum ESaveSlotStatus : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

UCLASS()
class AURA_API UVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeSlot() const;

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	
	/** Field Notifies */
	
	void SetLoadSlotName(const FString& InLoadSlotName) { UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName); }
	void SetPlayerName(const FText& InPlayerName) { UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName); }
	void SetSelectButtonIsEnabled(const bool& bInSelectButtonIsEnabled) { UE_MVVM_SET_PROPERTY_VALUE(bSelectButtonIsEnabled, bInSelectButtonIsEnabled); }
	void SetMapName(const FString& InMapName) { UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName); }
	
	FString GetLoadSlotName() const { return LoadSlotName; }
	FText GetPlayerName() const { return PlayerName; }
	bool GetSelectButtonIsEnabled() const { return bSelectButtonIsEnabled; }
	FString GetMapName() const { return MapName; }
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FText PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter = SetSelectButtonIsEnabled, Getter = GetSelectButtonIsEnabled, meta = (AllowPrivateAccess = "true"))
	bool bSelectButtonIsEnabled = true;
};
