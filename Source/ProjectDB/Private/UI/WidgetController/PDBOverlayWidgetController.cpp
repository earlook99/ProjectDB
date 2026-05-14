#include "UI/WidgetController/PDBOverlayWidgetController.h"

#include "AbilitySystem/PDBAttributeSet.h"
#include "AbilitySystemComponent.h"

void UPDBOverlayWidgetController::BroadcastInitialValues()
{
	const UPDBAttributeSet* PDBAttributeSet = CastChecked<UPDBAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(PDBAttributeSet->GetHealth());
}

void UPDBOverlayWidgetController::BindCallbacksToDependencies()
{
	const UPDBAttributeSet* PDBAttributeSet = CastChecked<UPDBAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PDBAttributeSet->GetHealthAttribute()).AddUObject(this, &UPDBOverlayWidgetController::HealthChanged);
}

void UPDBOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}
