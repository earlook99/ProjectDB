#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDBCharacterClassInfo.generated.h"

class UGameplayEffect;

UCLASS()
class PROJECTDB_API UPDBCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Common Defaults|Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Defaults|Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Defaults|Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;
};
