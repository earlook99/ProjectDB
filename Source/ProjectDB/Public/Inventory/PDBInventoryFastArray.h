#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PDBInventoryFastArray.generated.h"

class UPDBItemDataAsset;

USTRUCT(BlueprintType)
struct FPDBInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UPDBItemDataAsset> ItemDef;
	
	UPROPERTY()
	int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FPDBInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FPDBInventoryItem> Items;
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams) { return FFastArraySerializer::FastArrayDeltaSerialize<FPDBInventoryItem, FPDBInventoryList>(Items, DeltaParams, 
	*this); }
	
	void AddItem(UPDBItemDataAsset* ItemDef, int32 Count);
	void RemoveItem(UPDBItemDataAsset* ItemDef, int32 Count);

	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
};

template<>
struct TStructOpsTypeTraits<FPDBInventoryList> : public TStructOpsTypeTraitsBase2<FPDBInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
