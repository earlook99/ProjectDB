#include "Character/PDBCharacter.h"

#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "AbilitySystem/PDBAbilitySystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/PDBPlayerController.h"
#include "Player/PDBPlayerState.h"
#include "UI/HUD/PDBHUD.h"

APDBCharacter::APDBCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

void APDBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info for the server.
	InitAbilityActorInfo();
}

void APDBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info for the client.
	InitAbilityActorInfo();
}

void APDBCharacter::InitAbilityActorInfo()
{
	APDBPlayerState* PS = GetPlayerState<APDBPlayerState>();
	check(PS);

	AbilitySystemComponent = Cast<UPDBAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AttributeSet = PS->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	
	InitializeDefaultAttributes();

	if (APDBPlayerController* PC = Cast<APDBPlayerController>(GetController()))
	{
		if (APDBHUD* HUD = Cast<APDBHUD>(PC->GetHUD()))
		{
			HUD->InitOverlay(PC, PS, AbilitySystemComponent, AttributeSet);
		}
	}
}

void APDBCharacter::InitializeDefaultAttributes() const
{
	if (!HasAuthority())
	{
		return;
	}

	UPDBAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent);
}
