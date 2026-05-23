#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TargetType/PDBTargetType.h"
#include "PDBTargetType_Sphere.generated.h"

UCLASS()
class PROJECTDB_API UPDBTargetType_Sphere : public UPDBTargetType
{
	GENERATED_BODY()

public:
	virtual void ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const override;

private:
	UPROPERTY(EditAnywhere)
	float CircleRadius = 300.f;

	UPROPERTY(EditAnywhere)
	float CastRange = 600.f;
	
	UPROPERTY(EditAnywhere)
	float VerticalTolerance = 200.f;
};
