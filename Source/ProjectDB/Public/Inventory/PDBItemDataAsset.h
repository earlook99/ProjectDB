#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PDBItemDataAsset.generated.h"

class UGameplayEffect;

UCLASS()
class PROJECTDB_API UPDBItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackSize;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTag;
};
