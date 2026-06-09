#include "Inventory/PDBInventoryFastArray.h"

#include "Inventory/PDBItemDataAsset.h"   // ItemDef->DisplayName 등 접근에 완전 타입 필요
#include "Engine/Engine.h"                // GEngine->AddOnScreenDebugMessage

void FPDBInventoryList::AddItem(UPDBItemDataAsset* ItemDef, int32 Count)
{
	const int32 FoundIndex = Items.IndexOfByPredicate(
		[ItemDef](const FPDBInventoryItem& Entry){ return Entry.ItemDef == ItemDef; });
	if (FoundIndex != INDEX_NONE)
	{
		Items[FoundIndex].Quantity += Count;
		MarkItemDirty(Items[FoundIndex]);
	}
	else
	{
		FPDBInventoryItem& NewEntry = Items.AddDefaulted_GetRef();
		NewEntry.ItemDef = ItemDef;
		NewEntry.Quantity = Count;
		MarkItemDirty(NewEntry);
	}
}

void FPDBInventoryList::RemoveItem(UPDBItemDataAsset* ItemDef, int32 Count)
{
	
}

void FPDBInventoryList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (!GEngine) return;
	for (const auto& Index : AddedIndices)
	{
		const FString Name = Items[Index].ItemDef ? Items[Index].ItemDef->GetName() : TEXT("None");
		const int32 Quantity = Items[Index].Quantity;
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("%s x%d"), *Name, Quantity)
			);
	}
}

void FPDBInventoryList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	if (!GEngine) return;
	for (const auto& Index : ChangedIndices)
	{
		const FString Name = Items[Index].ItemDef ? Items[Index].ItemDef->GetName() : TEXT("None");
		const int32 Quantity = Items[Index].Quantity;
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("%s x%d"), *Name, Quantity)
			);
	}
}

void FPDBInventoryList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
}
