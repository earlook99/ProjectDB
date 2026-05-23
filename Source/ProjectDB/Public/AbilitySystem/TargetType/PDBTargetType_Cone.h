#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TargetType/PDBTargetType.h"
#include "PDBTargetType_Cone.generated.h"

UCLASS()
class PROJECTDB_API UPDBTargetType_Cone : public UPDBTargetType
{
	GENERATED_BODY()

public:
	virtual void ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const override;

private:
	UPROPERTY(EditAnywhere)
	float ConeRadius = 600.f;

	UPROPERTY(EditAnywhere)
	float ConeHalfAngle = 40.f;
};
