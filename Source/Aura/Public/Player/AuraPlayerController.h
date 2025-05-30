// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UNiagaraSystem;
class UNavigationSystemV1;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	mutable FName OriginalCollisionProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccluded = false;
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter, const bool bBlockedHit, const bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	/** How much of the Pawn capsule Radius and Height should be used for the Line Trace
	 * before considering an Actor occluded? Values too low may make the camera clip through walls
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float CapsulePercentageForTrace = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Materials")
	TObjectPtr<UMaterialInterface> FadeMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<USpringArmComponent> ActiveSpringArm;
	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<UCameraComponent> ActiveCamera;
	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	TObjectPtr<UCapsuleComponent> ActiveCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion")
	bool bIsOcclusionEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion")
	bool bDebugOcclusionLineTraces = false;

private:

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void AutoRun();

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetASC();

	UPROPERTY()
	TObjectPtr<UNavigationSystemV1> NavSystem;
	UPROPERTY(EditDefaultsOnly)
	bool bDebugNavEnabled = false;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputActionValue);
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bShiftKeyDown;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	int32 TargetSplinePointIdx = 0;

	float ControlledPawnHalfHeight = 0.f;

	bool GetCursorPlaneIntersection(const FVector& InPlaneOrigin, const FVector& InPlaneNormal, FVector& OutPlanePoint) const;
	bool GetScreenPositionPlaneIntersection(const FVector2D& ScreenPosition, const FVector& InPlaneOrigin, const FVector& InPlaneNormal, FVector& OutPlanePoint) const;

	void CursorTrace();
	FHitResult CursorHit;
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	float FollowTime;
	float ShortPressThreshold;
	bool bAutoRunning;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;
	bool bTargeting;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	TMap<const TObjectPtr<AActor>, FCameraOccludedActor> OccludedActors;

	bool HideOccludedActor(AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActors();
	FORCEINLINE bool ShouldCheckCameraOcclusion() const { return bIsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent; }
};
