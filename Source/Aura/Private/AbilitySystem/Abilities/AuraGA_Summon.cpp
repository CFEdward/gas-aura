// Copyright Eduard Ciofu


#include "AbilitySystem/Abilities/AuraGA_Summon.h"

TSubclassOf<APawn> UAuraGA_Summon::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}

TArray<FVector> UAuraGA_Summon::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	TArray<FVector> SpawnLocations;
	if (NumMinions < 1)
	{
		return SpawnLocations;
	}
	if (NumMinions == 1)
	{
		const float HalfSpawnSpread = SpawnSpread / 2.f;
		FVector ChosenSpawnLocation =
			Location +
			Forward * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance) +
			Forward.RotateAngleAxis(90.f, FVector::UpVector) * FMath::RandRange(-HalfSpawnSpread, HalfSpawnSpread);
		CheckGround(ChosenSpawnLocation);
		
		SpawnLocations.Add(ChosenSpawnLocation);
		return SpawnLocations;
	}
	
	const float DeltaSpread = SpawnSpread / (NumMinions - 1);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		CheckGround(ChosenSpawnLocation);
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

void UAuraGA_Summon::CheckGround(FVector& ChosenSpawnLocation) const
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
		ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
		ECC_Visibility
	);
	if (Hit.bBlockingHit)
	{
		ChosenSpawnLocation = Hit.ImpactPoint;
	}
}