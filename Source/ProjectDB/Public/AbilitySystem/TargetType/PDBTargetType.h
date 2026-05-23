#pragma once

#include "CoreMinimal.h"
#include "PDBTargetType.generated.h"

class AActor;
struct FHitResult;

UCLASS(Abstract, EditInlineNew, BlueprintType)
class PROJECTDB_API UPDBTargetType : public UObject
{
	GENERATED_BODY()

public:
	virtual void ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const;

protected:
	void GatherOverlapCandidates(AActor* Instigator, const FVector& Center, float Radius, TArray<AActor*>& OutActors) const;
};
