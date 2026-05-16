#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PDBGameplayAbility.h"

void UPDBAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	
}

void UPDBAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	TArray<FGameplayAbilitySpec>& AbilitySpecs = GetActivatableAbilities();
	
	ABILITYLIST_SCOPE_LOCK();
	
	for (FGameplayAbilitySpec& Spec : AbilitySpecs)
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UPDBAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{

}

void UPDBAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UPDBGameplayAbility>>& StartupAbilities)
{
	if (IsOwnerActorAuthoritative())
	{
		for (const TSubclassOf<UPDBGameplayAbility>& AbilityClass : StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			
			UPDBGameplayAbility* Ability = Cast<UPDBGameplayAbility>(AbilitySpec.Ability);
			if (Ability)
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->StartupInputTag);
			}
			
			GiveAbility(AbilitySpec);
		}
	}
}