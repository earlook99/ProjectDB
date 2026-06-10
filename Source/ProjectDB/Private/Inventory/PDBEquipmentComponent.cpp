#include "Inventory/PDBEquipmentComponent.h"

#include "Inventory/PDBEquipmentDataAsset.h"
#include "AbilitySystemBlueprintLibrary.h" 
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UPDBEquipmentComponent::UPDBEquipmentComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;
}

void UPDBEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPDBEquipmentComponent, EquippedItems);
}

void UPDBEquipmentComponent::Equip(UPDBEquipmentDataAsset* EquipDef)
{
	if (!GetOwner() || !(GetOwner()->HasAuthority())) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;
	
	const FGameplayTag Slot = EquipDef->EquipSlot;
	const int32 FoundIndex = EquippedItems.IndexOfByPredicate(
		[Slot](const FPDBEquipmentEntry& Entry) { return Entry.SlotTag == Slot; });
	if (FoundIndex != INDEX_NONE)
	{
		Unequip(Slot);
	}
	
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(EquipDef->EquipEffectClass, 1.f, ASC->MakeEffectContext());
	for (const FPDBEquipStatModifier& M : EquipDef->StatModifiers)
		Spec.Data->SetSetByCallerMagnitude(M.StatTag, M.Magnitude);
	
	FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	ActiveHandles.Add(Slot, Handle);
	
	EquippedItems.Add({Slot, EquipDef});
}

void UPDBEquipmentComponent::Unequip(FGameplayTag SlotTag)
{
	if (!GetOwner() || !(GetOwner()->HasAuthority())) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;
	
	FActiveGameplayEffectHandle* Found = ActiveHandles.Find(SlotTag);
	if (Found)
	{
		ASC->RemoveActiveGameplayEffect(*Found);
		ActiveHandles.Remove(SlotTag);
	}
	
	EquippedItems.RemoveAll([SlotTag](const FPDBEquipmentEntry& Entry) { return Entry.SlotTag == SlotTag; });
}

void UPDBEquipmentComponent::OnRep_EquippedItems()
{
}
