#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/PDBInventoryFastArray.h"   // FPDBInventoryList를 멤버로 보유 → 완전 타입 필요
#include "PDBInventoryComponent.generated.h"

class UPDBItemDataAsset;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class PROJECTDB_API UPDBInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPDBInventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void AddItem(UPDBItemDataAsset* ItemDef, int32 Count);
	
private:
	UPROPERTY(Replicated)
	FPDBInventoryList InventoryList;
};
