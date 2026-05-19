#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "PDBAnimNotify_SendGameplayEvent.generated.h"

UCLASS(meta = (DisplayName = "PDB Send Gameplay Event"))
class PROJECTDB_API UPDBAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
	virtual FString GetNotifyName_Implementation() const override;
#endif

protected:
	
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;
};
