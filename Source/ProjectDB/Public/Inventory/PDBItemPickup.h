#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDBItemPickup.generated.h"

class USphereComponent;
class UPDBItemDataAsset;

UCLASS()
class PROJECTDB_API APDBItemPickup : public AActor
{
	GENERATED_BODY()

public:
	APDBItemPickup();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const 
FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPDBItemDataAsset> ItemDef;
	
	UPROPERTY(EditAnywhere)
	int32 Quantity = 1;
};
