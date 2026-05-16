#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PDBAbilitySystemComponent.generated.h"

class UPDBGameplayAbility;

UCLASS()
class PROJECTDB_API UPDBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UPDBGameplayAbility>>& StartupAbilities);
};
