#include "AbilitySystem/Abilities/PDBActionAbility.h"
#include "GameplayTags/PDBGameplayTags.h"

UPDBActionAbility::UPDBActionAbility()
{
	ActivationOwnedTags.AddTag(FPDBGameplayTags::Get().State_Block_Movement);
}
