#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PDBPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class USplineComponent;

UCLASS()
class PROJECTDB_API APDBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APDBPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ClickAction;

	void OnInputPressed();
	void OnInputHeld();
	void OnInputReleased();

	void CursorTrace();
	void AutoRun();

	FHitResult CursorHit;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
