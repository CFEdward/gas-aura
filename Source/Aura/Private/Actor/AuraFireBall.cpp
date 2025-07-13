﻿// Copyright Eduard Ciofu


#include "Actor/AuraFireBall.h"

#include "GameFramework/ProjectileMovementComponent.h"


AAuraFireBall::AAuraFireBall()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovement->PrimaryComponentTick.bCanEverTick = false;
	ProjectileMovement->SetAutoActivate(false);
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
