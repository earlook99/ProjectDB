#include "Inventory/PDBInventoryComponent.h"

#include "Inventory/PDBItemDataAsset.h"
#include "Net/UnrealNetwork.h"   // DOREPLIFETIME

UPDBInventoryComponent::UPDBInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;
}

void UPDBInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPDBInventoryComponent, InventoryList);
}

void UPDBInventoryComponent::AddItem(UPDBItemDataAsset* ItemDef, int32 Count)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	InventoryList.AddItem(ItemDef, Count);
}
