#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PDBGameplayAbility.h"
#include "PDBActionAbility.generated.h"

USTRUCT(BlueprintType)
struct FPDBEffectContainer
{
	GENERATED_BODY()

	// TODO: 타깃팅 전략 필드 추가 — TObjectPtr<UPDBTargetType> (UPROPERTY에 EditAnywhere + Instanced).
	//   설계 §4.2: 컨테이너가 '누구한테(콘/구체)'를 데이터로 보유. 헤더 상단에 전방선언 `class UPDBTargetType;` 필요.

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
	
	// TODO(이전): 아래 ConeRadius/ConeHalfAngle은 UPDBTargetType_Cone의 멤버로 이사 → 여기서 제거.
	//   대신 EffectContainerMap의 각 컨테이너가 자기 TargetType(콘/구체)을 들고 간다.
	UPROPERTY(EditAnywhere)
	float ConeRadius = 600.f;

	UPROPERTY(EditAnywhere)
	float ConeHalfAngle = 40.f;
	
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	
	// TODO(이전): GatherConeTargets 본문은 UPDBTargetType_Cone::ResolveTargets로 이사 → 이 멤버 함수 제거.
	void GatherConeTargets(TArray<AActor*>& OutActors) const;

	// TODO: 커서 HitResult 캐싱 멤버 추가 (FHitResult 등).
	//   OnTargetDataReady에서 저장 → OnGameplayEventReceived(이벤트 시점)에서 Sphere가 ImpactPoint로 사용.
	//   현재는 회전에만 쓰고 버려서 이벤트 시점엔 커서 위치를 잃는다.

	UFUNCTION()
	void OnMontageCompleted();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
	UFUNCTION()
	void OnMontageCancelled();
	
	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
