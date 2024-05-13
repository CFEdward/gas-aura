// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:

	AAuraCharacter();

	//~ Begin ACharacter override
	virtual void PossessedBy(AController* NewController) override;
	//~ End ACharacter override
	
	//~ Begin APawn override
	virtual void OnRep_PlayerState() override;
	//~ End APawn override
	
private:

	void InitAbilityActorInfo();
	
};
