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

	GameplayTags.State_Block_Movement = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Block.Movement"),
		FString("Cannot move while ability is active")
	);
}
