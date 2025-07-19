// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

/* Object collision channels */
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

/* Trace collision channels */
#define ECC_Navigation ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel3
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel4