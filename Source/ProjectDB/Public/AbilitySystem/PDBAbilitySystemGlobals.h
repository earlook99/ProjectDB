#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PDBAbilitySystemGlobals.generated.h"

UCLASS()
class PROJECTDB_API UPDBAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
