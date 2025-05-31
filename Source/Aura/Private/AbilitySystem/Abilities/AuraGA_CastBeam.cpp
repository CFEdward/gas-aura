// Copyright Eduard Ciofu


#include "AbilitySystem/Abilities/AuraGA_CastBeam.h"

#include "GameFramework/Character.h"

bool UAuraGA_CastBeam::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
		return true;
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return false;
	}
}

void UAuraGA_CastBeam::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}
