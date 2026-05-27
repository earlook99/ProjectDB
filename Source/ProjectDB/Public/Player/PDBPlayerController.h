#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PDBPlayerController.generated.h"

class UPDBAbilitySystemComponent;
class UPDBInputConfig;
class UInputAction;
class UInputMappingContext;
class USplineComponent;
class IPDBEnemyInterface;

UCLASS()
class PROJECTDB_API APDBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APDBPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	
	UPDBAbilitySystemComponent* GetPDBAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UPDBInputConfig> InputConfig;
	
private:
	UPROPERTY()
	TObjectPtr<UPDBAbilitySystemComponent> AbilitySystemComponent;
	
	void CursorTrace();
	void AutoRun();
	void ChaseAndAutoAttack();
	
	bool IsMovementBlocked();

	FHitResult CursorHit;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	IPDBEnemyInterface* LastActor = nullptr;
	IPDBEnemyInterface* ThisActor = nullptr;
	
	TWeakObjectPtr<AActor> TargetActor = nullptr;
	
	float AttackRange = 150.f;
};
