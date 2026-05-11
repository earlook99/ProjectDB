#include "Player/PDBPlayerController.h"

#include "Components/SplineComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

APDBPlayerController::APDBPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void APDBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputMappingContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APDBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

	EIC->BindAction(ClickAction, ETriggerEvent::Started, this, &APDBPlayerController::OnInputPressed);
	EIC->BindAction(ClickAction, ETriggerEvent::Triggered, this, &APDBPlayerController::OnInputHeld);
	EIC->BindAction(ClickAction, ETriggerEvent::Completed, this, &APDBPlayerController::OnInputReleased);
}

void APDBPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	if (bAutoRunning) AutoRun();
}

void APDBPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
}

void APDBPlayerController::OnInputPressed()
{
	FollowTime = 0.f;
	bAutoRunning = false;
}

void APDBPlayerController::OnInputHeld()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDir = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDir);
		}
	}
}

void APDBPlayerController::OnInputReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
			GetWorld(), GetPawn()->GetActorLocation(), CachedDestination);
		
		if (NavigationPath && NavigationPath->PathPoints.Num() > 0)
		{
			Spline->ClearSplinePoints();
			
			for (const FVector& Point : NavigationPath->PathPoints)
			{
				Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
			}
			
			CachedDestination = NavigationPath->PathPoints.Last();
			
			bAutoRunning = true;
		}
	}
	
	FollowTime = 0.f;
}

void APDBPlayerController::AutoRun()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	
	FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	
	ControlledPawn->AddMovementInput(Direction);
	
	float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}
