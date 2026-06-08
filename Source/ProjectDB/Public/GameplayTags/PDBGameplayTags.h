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
	FGameplayTag InputTag_LMB;

	FGameplayTag State_Block_Movement;
	
	FGameplayTag GameplayCue_SoulSiphon_Cast;
	FGameplayTag GameplayCue_Subjugate_Cast;
	
	FGameplayTag Data_Damage;
	
private:
	static FPDBGameplayTags GameplayTags;
};
