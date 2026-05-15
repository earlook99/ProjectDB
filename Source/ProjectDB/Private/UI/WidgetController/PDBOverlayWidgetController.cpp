#include "UI/WidgetController/PDBOverlayWidgetController.h"

#include "AbilitySystem/PDBAttributeSet.h"
#include "AbilitySystemComponent.h"

void UPDBOverlayWidgetController::BroadcastInitialValues()
{
	const UPDBAttributeSet* PDBAttributeSet = CastChecked<UPDBAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(PDBAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PDBAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PDBAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PDBAttributeSet->GetMaxMana());
}

void UPDBOverlayWidgetController::BindCallbacksToDependencies()
{
	const UPDBAttributeSet* PDBAttributeSet = CastChecked<UPDBAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PDBAttributeSet->GetHealthAttribute()).AddUObject(
		this, &UPDBOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PDBAttributeSet->GetMaxHealthAttribute()).AddUObject(
		this, &UPDBOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PDBAttributeSet->GetManaAttribute()).AddUObject(
		this, &UPDBOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PDBAttributeSet->GetMaxManaAttribute()).AddUObject(
		this, &UPDBOverlayWidgetController::MaxManaChanged);
}

void UPDBOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UPDBOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UPDBOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UPDBOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
