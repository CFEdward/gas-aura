// Copyright Eduard Ciofu

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UObject/Interface.h"
#include "ModifierDependencyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModifierDependencyInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IModifierDependencyInterface
{
	GENERATED_BODY()

public:

	virtual FOnExternalGameplayModifierDependencyChange* GetOnModifierDependencyChanged() = 0;
};
