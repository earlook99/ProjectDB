#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PDBInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FPDBInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS()
class PROJECTDB_API UPDBInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FPDBInputAction> AbilityInputActions;
};
