#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "PDBEffectActor.generated.h"

UENUM(BlueprintType)
enum class EPDBEffectApplicationPolicy : uint8
{
	ApplyOnOverlap UMETA(DisplayName = "ApplyOnOverlap"),
	ApplyOnEndOverlap UMETA(DisplayName = "ApplyOnEndOverlap"),
	DoNotApply UMETA(DisplayName = "DoNotApply")
};

UENUM(BlueprintType)
enum class EPDBEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap UMETA(DisplayName = "RemoveOnEndOverlap"),
	DoNotRemove UMETA(DisplayName = "DoNotRemove")
};

class UGameplayEffect;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTDB_API APDBEffectActor : public AActor
{
	GENERATED_BODY()

public:
	APDBEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const 
FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Applied Effects")
	FActiveGameplayEffectHandle ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	// === Instant ===
	UPROPERTY(EditAnywhere, Category = "Applied Effects|Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effects|Instant")
	EPDBEffectApplicationPolicy InstantEffectApplicationPolicy = EPDBEffectApplicationPolicy::DoNotApply;

	// === Duration ===
	UPROPERTY(EditAnywhere, Category = "Applied Effects|Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effects|Duration")
	EPDBEffectApplicationPolicy DurationEffectApplicationPolicy = EPDBEffectApplicationPolicy::DoNotApply;

	// === Infinite ===
	UPROPERTY(EditAnywhere, Category = "Applied Effects|Infinite")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effects|Infinite")
	EPDBEffectApplicationPolicy InfiniteEffectApplicationPolicy = EPDBEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, Category = "Applied Effects|Infinite")
	EPDBEffectRemovalPolicy InfiniteEffectRemovalPolicy = EPDBEffectRemovalPolicy::RemoveOnEndOverlap;
	
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent>> ActiveGameplayEffects;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	float ActorLevel = 1.0f;
};
