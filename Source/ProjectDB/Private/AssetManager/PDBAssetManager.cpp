#include "AssetManager/PDBAssetManager.h"
#include "GameplayTags/PDBGameplayTags.h"

UPDBAssetManager& UPDBAssetManager::Get()
{
	check(GEngine);
	UPDBAssetManager* AssetManager = Cast<UPDBAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void UPDBAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FPDBGameplayTags::InitializeNativeGameplayTags();
}
