#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PDBGameplayTags.generated.h"

USTRUCT()
struct PROJECTDB_API FPDBGameplayTags
{
	GENERATED_BODY()
	
public:
	static const FPDBGameplayTags& Get() { return GameplayTags; }
	
	static void InitializeNativeGameplayTags();
	
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;

	FGameplayTag State_Block_Movement;
	
private:
	static FPDBGameplayTags GameplayTags;
};
