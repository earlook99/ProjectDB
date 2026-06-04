#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "Interaction/PDBCombatInterface.h"
#include "PDBCharacterBase.generated.h"

class UPDBGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;

UCLASS(Abstract)
class PROJECTDB_API APDBCharacterBase : public ACharacter, public IAbilitySystemInterface, public IPDBCombatInterface
{
	GENERATED_BODY()

public:
	APDBCharacterBase();

	virtual UPDBAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	virtual void Die() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UPDBAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UPDBGameplayAbility>> StartupAbilities;
};
