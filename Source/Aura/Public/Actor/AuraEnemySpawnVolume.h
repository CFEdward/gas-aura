// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:

	AAuraEnemySpawnVolume();

	/** Save Interface */
	virtual void LoadActor_Implementation() override;
	/** end Save Interface */

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	bool bReached = false;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;
};
