#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PDBPlayerState.generated.h"

class UPDBEquipmentComponent;
class UPDBInventoryComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROJECTDB_API APDBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APDBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	UPDBInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<UPDBInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	TObjectPtr<UPDBEquipmentComponent> EquipmentComponent;
};
