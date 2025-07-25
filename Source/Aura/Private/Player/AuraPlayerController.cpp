// Copyright Eduard Ciofu


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/AuraEnemySpawnVolume.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Elements/Framework/TypedElementSorter.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController() :
	bShiftKeyDown(false), FollowTime(0.f), ShortPressThreshold(0.5f), bAutoRunning(false),
	AutoRunAcceptanceRadius(50.f)
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem*	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::SetShowMouseCursorAndForceRefresh(bool bNewValue)
{
	SetShowMouseCursor(bNewValue);
	// Force the cursor to refresh by setting the mouse position to itself
	float XMouseLocation;
	float YMouseLocation;
	GetMousePosition(XMouseLocation, YMouseLocation);
	SetMouseLocation(XMouseLocation, YMouseLocation);
}

void AAuraPlayerController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, const bool bBlockedHit, const bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector PawnLocation = ControlledPawn->GetActorLocation();
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PawnLocation, ESplineCoordinateSpace::World);
		const FVector TargetSplinePointLocation = Spline->GetLocationAtSplinePoint(TargetSplinePointIdx, ESplineCoordinateSpace::World);
		FVector WorldDirection = TargetSplinePointLocation - PawnLocation;
		WorldDirection.Z = 0.f;
		// NOTE: We get the Normal after zeroing Z to get a constant movement speed along the XY plane
		WorldDirection = WorldDirection.GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
		
		const float DistanceToTarget = (LocationOnSpline - TargetSplinePointLocation).Length();
		if (DistanceToTarget <= AutoRunAcceptanceRadius)
		{
			if (const bool bNextTargetPointExist = TargetSplinePointIdx < Spline->GetNumberOfSplinePoints() - 1)
			{
				TargetSplinePointIdx++;
			}
			else
			{
				bAutoRunning = false;
			}
		}

		if (bDebugNavEnabled)
		{
			for (int32 SplinePointIdx = 0; SplinePointIdx < Spline->GetNumberOfSplinePoints(); ++SplinePointIdx)
			{
				const FVector SplinePointLocation = Spline->GetLocationAtSplinePoint(SplinePointIdx, ESplineCoordinateSpace::World);
				if (SplinePointIdx > 0)
				{
					const FVector PreviousSplinePointLocation = Spline->GetLocationAtSplinePoint(SplinePointIdx - 1, ESplineCoordinateSpace::World);
					DrawDebugLine(GetWorld(), PreviousSplinePointLocation, SplinePointLocation, FColor::Red);
				}
				DrawDebugSphere(GetWorld(), LocationOnSpline, 20.f, 12, FColor::Cyan);

				const FVector LineStart = PawnLocation + WorldDirection.GetSafeNormal() * 50.f;
				const FVector LineEnd = LineStart + WorldDirection * 100.f;
				UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd, 20.f, FLinearColor::Yellow, 0.f, 4.f);

				DrawDebugSphere(GetWorld(), TargetSplinePointLocation, 20.f, 12, FColor::Yellow);

				UE_LOG(LogTemp, Warning, TEXT("TargetSplinePointIdx: %i, DistanceToTarget: %f"), TargetSplinePointIdx, DistanceToTarget);
			}
		}
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed)) return;
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		ControlledPawnHalfHeight = Cast<ICombatInterface>(GetPawn())->GetHalfHeight();
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingObject;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased)) return;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const bool bIsAction = InputTag.MatchesTagExact(GameplayTags.InputTag_LMB);
	const bool bIsWaiting = GetASC()->HasMatchingGameplayTag(GameplayTags.Abilities_Status_WaitingExecution);
	
	if (!bIsAction)
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (!bIsWaiting && TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown && GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// Set move-to location
			// Targeted non-enemy's move-to location
			bool bMoveToLocationSet = false;
			bool bShouldShowClickVFX = false;
			if (IsValid(ThisActor))
			{
				IHighlightInterface* HighlightInterface = Cast<IHighlightInterface>(ThisActor);
				if (HighlightInterface)
				{
					MoveToLocation = HighlightInterface->GetMoveToLocation();
					bMoveToLocationSet = true;
				}
			}
			// Clicked location
			if (!bMoveToLocationSet)
			{
				FHitResult NavChannelCursorHitResult;
				GetHitResultUnderCursor(ECC_Navigation, false, NavChannelCursorHitResult);
				if (NavChannelCursorHitResult.bBlockingHit)
				{
					// Projecting a point from the cursor impact point to the NavMesh with a larger-than-default
					// Query Extent, so there are better chances to reach for the NavMesh and return a point,
					// then generating a path from the Pawn location to this point (only if found)
					
					FNavLocation ImpactPointNavLocation;
					// NOTE: Default Query Extent = FVector(50.f, 50.f, 250.f)
					const FVector QueryingExtent = FVector(400.f, 400.f, 250.f);
					const FNavAgentProperties& NavAgentProps = GetNavAgentPropertiesRef();
					const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(
						NavChannelCursorHitResult.ImpactPoint,
						ImpactPointNavLocation,
						QueryingExtent,
						&NavAgentProps
					);
					if (bNavLocationFound)
					{
						MoveToLocation = ImpactPointNavLocation;
						bMoveToLocationSet = true;
						bShouldShowClickVFX = true;
					}
					
					if (bDebugNavEnabled)
					{
						DrawDebugBox(GetWorld(), NavChannelCursorHitResult.ImpactPoint, QueryingExtent, FColor::Silver, false, 3.f);
					}
				}
			}

			// Find path to move-to location and enable auto run
			if (bMoveToLocationSet)
			{
				UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
					this,
					ControlledPawn->GetActorLocation(),
					MoveToLocation
				);
				if (NavPath && NavPath->PathPoints.Num() > 0)
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					}
					TargetSplinePointIdx = 1;
					bAutoRunning = true;

					if (bShouldShowClickVFX && GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, MoveToLocation);
					}
				}
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld)) return;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const bool bIsAction = InputTag.MatchesTagExact(GameplayTags.InputTag_LMB);
	const bool bIsWaiting = GetASC()->HasMatchingGameplayTag(GameplayTags.Abilities_Status_WaitingExecution);
	
	if (!bIsAction)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bIsAction && !(TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown || bIsWaiting))
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector PawnLocation = ControlledPawn->GetActorLocation();
			FVector PawnBottomLocation = PawnLocation;
			PawnBottomLocation.Z -= ControlledPawnHalfHeight;
			FVector CursorHorizPlaneIntersection;
			if (const bool bIntersectionFound = GetCursorPlaneIntersection(PawnBottomLocation, FVector::UpVector, CursorHorizPlaneIntersection))
			{
				FVector WorldDirection = (CursorHorizPlaneIntersection - PawnLocation).GetSafeNormal();
				WorldDirection.Z = 0.f;
				ControlledPawn->AddMovementInput(WorldDirection);

				if (bDebugNavEnabled)
				{
					DrawDebugSphere(GetWorld(), CursorHorizPlaneIntersection, 20.f, 12, FColor::Green);
					const FVector LineStart = PawnLocation + WorldDirection.GetSafeNormal() * 50.f;
					const FVector LineEnd = LineStart + WorldDirection * 100.f;
					UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd, 20.f, FLinearColor::Green, 0.f, 4.f);
				}
				bAutoRunning = false;
			}
		}
	}
	else
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	//if (bTargeting || bShiftKeyDown)
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed)) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		bAutoRunning = false;
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

bool AAuraPlayerController::GetCursorPlaneIntersection(const FVector& InPlaneOrigin, const FVector& InPlaneNormal, FVector& OutPlanePoint) const
{
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player); LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (const bool bMousePositionFound = LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			return GetScreenPositionPlaneIntersection(MousePosition, InPlaneOrigin, InPlaneNormal, OutPlanePoint);
		}
	}

	return false;
}

bool AAuraPlayerController::GetScreenPositionPlaneIntersection(const FVector2D& ScreenPosition, const FVector& InPlaneOrigin, const FVector& InPlaneNormal, FVector& OutPlanePoint) const
{
	// Early out if we clicked on a HUD hitbox
	if (const AHUD* HUD = GetHUD(); HUD && HUD->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}

	FVector WorldOrigin;
	FVector WorldDirection;
	if (const bool bScreenPositionDeprojected = UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection))
	{
		OutPlanePoint = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, InPlaneOrigin, InPlaneNormal);
		return true;
	}

	return false;
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) IHighlightInterface::Execute_HighlightActor(LastActor);
		if (ThisActor) IHighlightInterface::Execute_UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		if (LastActor) IHighlightInterface::Execute_UnHighlightActor(LastActor);
		if (ThisActor) IHighlightInterface::Execute_HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;

	// Camera is currently colliding, show all current occluded actors
	// and do not perform further occlusion
	if (ActiveSpringArm->bDoCollisionTest)
	{
		ForceShowOccludedActors();
		return;
	}

	const FVector Start = ActiveCamera->GetComponentLocation();
	const FVector End = GetPawn()->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;	// TODO: Add configuration to ignore actor types
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVolume::StaticClass(), ActorsToIgnore);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAuraEnemySpawnVolume::StaticClass(), ActorsToIgnore);
	
	TArray<FHitResult> OutHits;

	const auto ShouldDebug = bDebugOcclusionLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	const bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
		CollisionObjectTypes,
		true,
		ActorsToIgnore,
		ShouldDebug,
		OutHits,
		true
	);

	if (bGotHits)
	{
		// The list of actors hit by the line trace, that means that they are occluded from view
		TSet<const AActor*> ActorsJustOccluded;

		// Hide actors that are occluded by the camera
		for (FHitResult Hit : OutHits)
		{
			if (Hit.GetActor()->ActorHasTag(FName("IgnoreOcclusion"))) continue;
			AActor* HitActor = Cast<AActor>(Hit.GetActor());
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}

		// Show actors that are currently hidden but that are not occluded by the camera anymore
		for (auto& Elem : OccludedActors)
		{
			if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.bIsOccluded)
			{
				ShowOccludedActor(Elem.Value);

				if (bDebugOcclusionLineTraces)
					UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
			}
		}
	}
	else
	{
		ForceShowOccludedActors();
	}
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		const FVector MagicCircleLoc = CursorHit.ImpactPoint;
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, MagicCircleLoc, FRotator::ZeroRotator);
		if (DecalMaterial)
		{
			MagicCircle->GetMagicCircleDecalComp()->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle() const
{
	if (IsValid(MagicCircle)) MagicCircle->Destroy();
}

void AAuraPlayerController::UpdateMagicCircleLocation() const
{
	if (IsValid(MagicCircle))
	{
		if (CursorHit.bBlockingHit)
		{
			MagicCircle->SetActorHiddenInGame(false);
			MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
		}
		else
		{
			MagicCircle->SetActorHiddenInGame(true);
		}
	}
}

bool AAuraPlayerController::HideOccludedActor(AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

	if (ExistingOccludedActor && ExistingOccludedActor->bIsOccluded)
	{
		if (bDebugOcclusionLineTraces)
			UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."), *Actor->GetName());
		return false;
	}

	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->bIsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);

		if (bDebugOcclusionLineTraces)
			UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
	}
	else
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMesh = StaticMesh;
		OccludedActor.Materials = StaticMesh->GetMaterials();
		OccludedActor.bIsOccluded = true;
		OccludedActor.OriginalCollisionProfile = StaticMesh->GetCollisionProfileName();
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);

		if (bDebugOcclusionLineTraces)
			UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."), *Actor->GetName());
	}

	return true;
}

bool AAuraPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Navigation, ECR_Ignore);
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
	}

	return true;
}

void AAuraPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if (!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}
	//OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	OccludedActor.bIsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

bool AAuraPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	OccludedActor.StaticMesh->SetCollisionProfileName(OccludedActor.OriginalCollisionProfile);
	for (int MatIdx = 0; MatIdx < OccludedActor.Materials.Num(); ++MatIdx)
	{
		OccludedActor.StaticMesh->SetMaterial(MatIdx, OccludedActor.Materials[MatIdx]);
	}

	return true;
}

void AAuraPlayerController::ForceShowOccludedActors()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.bIsOccluded)
		{
			ShowOccludedActor(Elem.Value);

			if (bDebugOcclusionLineTraces)
				UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."), *Elem.Value.Actor->GetName());
		}
	}
}
