// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadMenuSaveGame;
class USaveGame;
class UVM_LoadSlot;

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	static void DeleteSlot(const UVM_LoadSlot* LoadSlot);
	
	void SaveSlotData(const UVM_LoadSlot* LoadSlot, const int32 SlotIndex) const;
	ULoadMenuSaveGame* GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const;

	void TravelToMap(const UVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

protected:

	virtual void BeginPlay() override;
};
