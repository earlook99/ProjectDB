#include "Input/PDBInputConfig.h"

const UInputAction* UPDBInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FPDBInputAction& Action : AbilityInputActions)
	{
		if (!Action.InputAction) continue;
		if (Action.InputTag.MatchesTagExact(InputTag)) return Action.InputAction;
	}
	
	return nullptr;
}
