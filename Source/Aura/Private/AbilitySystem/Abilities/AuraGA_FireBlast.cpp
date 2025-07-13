// Copyright Eduard Ciofu


#include "AbilitySystem/Abilities/AuraGA_FireBlast.h"

UAuraGA_FireBlast::UAuraGA_FireBlast()
{
	MaxNumSubSpell = 12;
}

TArray<AAuraFireBall*> UAuraGA_FireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
