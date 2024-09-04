// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:

	AAuraEnemy();
	
	//~ Begin IEnemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End IEnemy Interface

protected:

	//~ Begin ACharacter override
	virtual void BeginPlay() override;
	//~ End ACharacter override

	virtual void InitAbilityActorInfo() override;
	
};
