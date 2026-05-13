#include "Character/PDBCharacterBase.h"

APDBCharacterBase::APDBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* APDBCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APDBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
