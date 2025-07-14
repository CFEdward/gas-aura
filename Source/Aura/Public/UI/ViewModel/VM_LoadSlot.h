// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

UCLASS()
class AURA_API UVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeSlot();

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	FString PlayerName;

	/** Field Notifies */
	//UFUNCTION(BlueprintSetter)
	void SetLoadSlotName(const FString& InLoadSlotName);
	//UFUNCTION(BlueprintGetter)
	FString GetLoadSlotName() const { return LoadSlotName; }

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
};
