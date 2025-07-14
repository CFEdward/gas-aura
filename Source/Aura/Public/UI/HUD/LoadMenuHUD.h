// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadMenuHUD.generated.h"

class UVM_LoadMenu;
class ULoadMenuWidget;

UCLASS()
class AURA_API ALoadMenuHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadMenuWidget> LoadMenuWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadMenuWidget> LoadMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_LoadMenu> LoadMenuVMClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UVM_LoadMenu> LoadMenuVM;

protected:

	virtual void BeginPlay() override;
};
