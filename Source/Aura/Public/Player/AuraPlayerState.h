// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/ModifierDependencyInterface.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /* StatValue */, bool /* bLevelUp */);

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface, public IModifierDependencyInterface
{
	GENERATED_BODY()

public:

	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** ModifierDependency Interface */
	FORCEINLINE virtual FOnExternalGameplayModifierDependencyChange* GetOnModifierDependencyChanged() override { return &OnModifierDependencyChanged; }
	/** end ModifierDependency Interface */
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStatChanged OnXPChangedDelegate;
	void AddToXP(const int32 InXP);
	void SetXP(const int32 InXP);

	FOnLevelChanged OnLevelChangedDelegate;
	void AddToLevel(const int32 InLevel);
	void SetLevel(const int32 InLevel);

	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	void AddToAttributePoints(const int32 InAttributePoints);
	void SetAttributePoints(const int32 InAttributePoints);
	
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;
	void AddToSpellPoints(const int32 InSpellPoints);
	void SetSpellPoints(const int32 InSpellPoints);
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	FOnExternalGameplayModifierDependencyChange OnModifierDependencyChanged;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
public:

	/** AbilitySystem Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** end AbilitySystem Interface */
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP;}
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
};
