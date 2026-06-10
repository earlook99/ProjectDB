#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "PDBEquipmentComponent.generated.h"

class UPDBItemDataAsset;
class UPDBEquipmentDataAsset;

USTRUCT(BlueprintType)
struct FPDBEquipmentEntry
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGameplayTag SlotTag;
	
	UPROPERTY()
	TObjectPtr<UPDBItemDataAsset> ItemDef;
};

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class PROJECTDB_API UPDBEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPDBEquipmentComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedItems)
	TArray<FPDBEquipmentEntry> EquippedItems;
	
	TMap<FGameplayTag, FActiveGameplayEffectHandle> ActiveHandles;
	
	void Equip(UPDBEquipmentDataAsset* EquipDef);
	void Unequip(FGameplayTag SlotTag);
	
	UFUNCTION()
	void OnRep_EquippedItems();
};
