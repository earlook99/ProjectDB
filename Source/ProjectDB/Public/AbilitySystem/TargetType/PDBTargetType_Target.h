#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TargetType/PDBTargetType.h"
#include "PDBTargetType_Target.generated.h"

UCLASS()
class PROJECTDB_API UPDBTargetType_Target : public UPDBTargetType
{
	GENERATED_BODY()
	
public:
	virtual void ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const override;
};
