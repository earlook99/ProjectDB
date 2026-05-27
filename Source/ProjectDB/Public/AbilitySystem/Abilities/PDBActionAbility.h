#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PDBGameplayAbility.h"
#include "PDBActionAbility.generated.h"

class UPDBTargetType;

USTRUCT(BlueprintType)
struct FPDBEffectContainer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UPDBTargetType> TargetType;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;
};

UENUM(BlueprintType)
enum class EPDBCueOrigin : uint8
{
	Caster UMETA(DisplayName = "Caster"),
	Cursor UMETA(DisplayName = "Cursor"),
	Custom UMETA(DisplayName = "Custom")
};

UCLASS()
class PROJECTDB_API UPDBActionAbility : public UPDBGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPDBActionAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetCustomCueLocation();
	
	UPROPERTY(BlueprintReadOnly)
	FHitResult CursorHit;
	
private:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FPDBEffectContainer> EffectContainerMap;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag CastCueTag;
	
	UPROPERTY(EditAnywhere)
	EPDBCueOrigin CueOrigin = EPDBCueOrigin::Caster;
	
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageCompleted();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
	UFUNCTION()
	void OnMontageCancelled();
	
	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
