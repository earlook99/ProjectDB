#pragma once

#include "CoreMinimal.h"
#include "Inventory/PDBItemDataAsset.h"
#include "PDBEquipmentDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FPDBEquipStatModifier
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StatTag;
	
	UPROPERTY(EditDefaultsOnly)
	float Magnitude;
};

UCLASS()
class PROJECTDB_API UPDBEquipmentDataAsset : public UPDBItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag EquipSlot;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EquipEffectClass;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FPDBEquipStatModifier> StatModifiers;
};
