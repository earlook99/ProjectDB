#include "Character/PDBCharacterBase.h"

APDBCharacterBase::APDBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APDBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
