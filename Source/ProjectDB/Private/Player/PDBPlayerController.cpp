#include "Player/PDBPlayerController.h"

#include "Components/SplineComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Input/PDBInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Interaction/PDBEnemyInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "GameplayTags/PDBGameplayTags.h"
#include "Player/PDBPlayerState.h"

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
	UPDBInputComponent* PIC = CastChecked<UPDBInputComponent>(InputComponent);

	PIC->BindAction(ClickAction, ETriggerEvent::Started, this, &APDBPlayerController::OnInputPressed);
	PIC->BindAction(ClickAction, ETriggerEvent::Triggered, this, &APDBPlayerController::OnInputHeld);
	PIC->BindAction(ClickAction, ETriggerEvent::Completed, this, &APDBPlayerController::OnInputReleased);
	
	PIC->BindAbilityActions(
		InputConfig, 
		this, 
		&APDBPlayerController::AbilityInputTagPressed,
		&APDBPlayerController::AbilityInputTagHeld,
		&APDBPlayerController::AbilityInputTagReleased
		);
}

void APDBPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	if (bAutoRunning) AutoRun();
}

UPDBAbilitySystemComponent* APDBPlayerController::GetPDBAbilitySystemComponent()
{
	if (!AbilitySystemComponent)
	{
		APDBPlayerState* PS = GetPlayerState<APDBPlayerState>();
		if (PS)
		{
			AbilitySystemComponent = Cast<UPDBAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		}
	}
	
	return AbilitySystemComponent;
}

void APDBPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->AbilityInputTagPressed(InputTag);
}

void APDBPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->AbilityInputTagHeld(InputTag);
}

void APDBPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->AbilityInputTagReleased(InputTag);
}

void APDBPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IPDBEnemyInterface>(CursorHit.GetActor());

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
			ThisActor->HighlightActor();
	}
	else
	{
		if (ThisActor == nullptr)
			LastActor->UnHighlightActor();
		else if (LastActor != ThisActor)
		{
			LastActor->UnHighlightActor();
			ThisActor->HighlightActor();
		}
	}
}

void APDBPlayerController::OnInputPressed()
{
	FollowTime = 0.f;
	bAutoRunning = false;
}

void APDBPlayerController::OnInputHeld()
{
	if (IsMovementBlocked()) return;
	
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
	if (IsMovementBlocked()) return;
	
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
	if (IsMovementBlocked()) return;
	
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

bool APDBPlayerController::IsMovementBlocked()
{
	if (!GetPDBAbilitySystemComponent()) return false;
	return GetPDBAbilitySystemComponent()->HasMatchingGameplayTag(FPDBGameplayTags::Get().State_Block_Movement);
}
