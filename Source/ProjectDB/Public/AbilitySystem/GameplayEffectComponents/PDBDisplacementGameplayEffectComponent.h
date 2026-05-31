#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "PDBDisplacementGameplayEffectComponent.generated.h"

UCLASS()
class PROJECTDB_API UPDBDisplacementGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	virtual void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

private:
	UPROPERTY(EditAnywhere)
	float KnockbackMagnitude = 600.f;
};
