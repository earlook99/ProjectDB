#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PDBGameplayAbility.h"
#include "PDBActionAbility.generated.h"

USTRUCT(BlueprintType)
struct FPDBEffectContainer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;
};

UCLASS()
class PROJECTDB_API UPDBActionAbility : public UPDBGameplayAbility
{
	GENERATED_BODY()

	UPDBActionAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FPDBEffectContainer> EffectContainerMap;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditAnywhere)
	float ConeRadius = 600.f;
	
	UPROPERTY(EditAnywhere)
	float ConeHalfAngle = 40.f;
	
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	
	void GatherConeTargets(TArray<AActor*>& OutActors) const;

	UFUNCTION()
	void OnMontageCompleted();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
	UFUNCTION()
	void OnMontageCancelled();
	
	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
