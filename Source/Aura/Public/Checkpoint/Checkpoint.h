// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:

	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	/** Save Interface */
	virtual void LoadActor_Implementation() override;
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	/** end Save Interface */

	/** Highlight Interface */
	virtual FVector GetMoveToLocation() override { return MoveToComponent->GetComponentLocation(); }
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/** end Highlight Interface */
	
	UPROPERTY(SaveGame)
	bool bReached = false;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
private:
	
	UPROPERTY()
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditDefaultsOnly)
	EStencilValue StencilValue = EStencilValue::Tan;
};
