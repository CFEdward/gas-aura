// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250

/* Object collision channels */
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

/* Trace collision channels */
#define ECC_Navigation ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel3