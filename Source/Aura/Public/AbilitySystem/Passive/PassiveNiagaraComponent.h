// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "PassiveNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	
	UPassiveNiagaraComponent();
	void ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC);

protected:
	
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate);
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;
};
