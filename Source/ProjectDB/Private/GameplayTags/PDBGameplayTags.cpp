#include "GameplayTags/PDBGameplayTags.h"

#include "GameplayTagsManager.h"

FPDBGameplayTags FPDBGameplayTags::GameplayTags;

void FPDBGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Q")
	);
	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.W"),
		FString("W")
	);
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("E")
	);
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("R")
	);
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("LMB")
	);

	GameplayTags.State_Block_Movement = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Block.Movement"),
		FString("Cannot move while ability is active")
	);
	
	GameplayTags.GameplayCue_SoulSiphon_Cast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.SoulSiphon.Cast"),
		FString("Casting SoulSiphon")
		);
	
	GameplayTags.GameplayCue_Subjugate_Cast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Subjugate.Cast"),
		FString("Casting Subjugate")
		);
	
	GameplayTags.Data_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.Damage"),
		FString("SetByCaller Damage Amount")
		);
}
