#include "AbilitySystem/PDBAbilitySystemGlobals.h"
#include "AbilitySystem/PDBGameplayEffectContext.h"

FGameplayEffectContext* UPDBAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FPDBGameplayEffectContext();
}
