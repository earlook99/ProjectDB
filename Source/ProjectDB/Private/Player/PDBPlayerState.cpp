#include "Player/PDBPlayerState.h"

#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "AbilitySystem/PDBAttributeSet.h"
#include "Inventory/PDBInventoryComponent.h"

APDBPlayerState::APDBPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPDBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPDBAttributeSet>(TEXT("AttributeSet"));

	InventoryComponent = CreateDefaultSubobject<UPDBInventoryComponent>(TEXT("InventoryComponent"));

	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* APDBPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
