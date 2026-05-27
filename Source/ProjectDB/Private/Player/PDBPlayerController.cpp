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
	
	if (TargetActor.IsValid()) ChaseAndAutoAttack();
	else if (bAutoRunning) AutoRun();
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
	if (!InputTag.MatchesTagExact(FPDBGameplayTags::Get().InputTag_LMB))
	{
		UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();                                                                 
		if (!ASC) return;                                               
		ASC->AbilityInputTagPressed(InputTag);
	}
	else
	{
		FollowTime = 0.f;
		bAutoRunning = false;
		
		if (ThisActor)
		{
			TargetActor = CursorHit.GetActor();
		}
		else
		{
			TargetActor = nullptr;
		}
	}
}

void APDBPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FPDBGameplayTags::Get().InputTag_LMB))
	{
		UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();                                                                 
		if (!ASC) return;                                               
		ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		if (TargetActor.IsValid()) return;
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
}

void APDBPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FPDBGameplayTags::Get().InputTag_LMB))
	{
		UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();                                                                 
		if (!ASC) return;                                               
		ASC->AbilityInputTagReleased(InputTag);
	}
	else
	{
		if (TargetActor.IsValid()) return;
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

void APDBPlayerController::AutoRun()
{
	if (IsMovementBlocked())
	{
		bAutoRunning = false;
		return;
	}
	
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

void APDBPlayerController::ChaseAndAutoAttack()
{
	if (!GetPawn()) return;
			
	float Distance = FVector::Dist(TargetActor->GetActorLocation(), GetPawn()->GetActorLocation());
			
	if (AttackRange < Distance)
	{
		if (IsMovementBlocked()) return;
			
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDir = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDir);
		}
	}
	else
	{
		UPDBAbilitySystemComponent* ASC = GetPDBAbilitySystemComponent();                                                                 
		if (!ASC) return;                                               
		ASC->AbilityInputTagHeld(FPDBGameplayTags::Get().InputTag_LMB);
		TargetActor = nullptr;
	}
}

bool APDBPlayerController::IsMovementBlocked()
{
	if (!GetPDBAbilitySystemComponent()) return false;
	return GetPDBAbilitySystemComponent()->HasMatchingGameplayTag(FPDBGameplayTags::Get().State_Block_Movement);
}
