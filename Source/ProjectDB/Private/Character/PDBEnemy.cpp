#include "Character/PDBEnemy.h"

#include "AbilitySystem/PDBAbilitySystemComponent.h"
#include "AbilitySystem/PDBAbilitySystemLibrary.h"
#include "AbilitySystem/PDBAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "ProjectDB/ProjectDB.h"
#include "UI/Widget/PDBUserWidget.h"

APDBEnemy::APDBEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UPDBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UPDBAttributeSet>(TEXT("AttributeSet"));
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void APDBEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	UPDBUserWidget* UserWidget = Cast<UPDBUserWidget>(HealthBar->GetUserWidgetObject());
	if (UserWidget)
	{
		UserWidget->SetWidgetController(this);
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPDBAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPDBAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	
	if (!HasAuthority()) return;
	UPDBAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent);
	
	UPDBAttributeSet* PDBAttributeSet = Cast<UPDBAttributeSet>(AttributeSet.Get());
	
	OnHealthChanged.Broadcast(PDBAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PDBAttributeSet->GetMaxHealth());
}

void APDBEnemy::HighlightActor()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void APDBEnemy::UnHighlightActor()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
}
