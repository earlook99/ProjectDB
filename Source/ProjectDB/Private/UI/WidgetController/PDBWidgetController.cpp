#include "UI/WidgetController/PDBWidgetController.h"

void UPDBWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UPDBWidgetController::BroadcastInitialValues()
{
}

void UPDBWidgetController::BindCallbacksToDependencies()
{
}
