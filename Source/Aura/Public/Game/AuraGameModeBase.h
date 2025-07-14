// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class USaveGame;
class UVM_LoadSlot;

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	void SaveSlotData(UVM_LoadSlot* LoadSlot, const int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;
};
