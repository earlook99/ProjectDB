#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDBEffectActor.generated.h"

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

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	float ActorLevel = 1.0f;
};
