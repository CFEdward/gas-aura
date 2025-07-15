// Copyright Eduard Ciofu


#include "UI/HUD/LoadMenuHUD.h"

#include "UI/ViewModel/VM_LoadMenu.h"
#include "UI/Widget/LoadMenuWidget.h"

void ALoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadMenuVM = NewObject<UVM_LoadMenu>(this, LoadMenuVMClass);
	LoadMenuVM->InitializeLoadSlots();

	LoadMenuWidget = CreateWidget<ULoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();

	LoadMenuVM->LoadData();
}
