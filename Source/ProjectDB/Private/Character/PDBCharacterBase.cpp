#include "Character/PDBCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APDBCharacterBase::APDBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UPDBAbilitySystemComponent* APDBCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APDBCharacterBase::Die()
{
	MulticastHandleDeath();
}

void APDBCharacterBase::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void APDBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
