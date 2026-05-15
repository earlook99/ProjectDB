#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PDBAssetManager.generated.h"

UCLASS()
class PROJECTDB_API UPDBAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UPDBAssetManager& Get();
	
protected:
	virtual void StartInitialLoading() override;
};
