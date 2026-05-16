#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PDBGameplayAbility.generated.h"

UCLASS()
class PROJECTDB_API UPDBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
