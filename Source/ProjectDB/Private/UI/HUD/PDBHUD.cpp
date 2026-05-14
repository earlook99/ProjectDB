#include "UI/HUD/PDBHUD.h"

#include "UI/Widget/PDBUserWidget.h"
#include "UI/WidgetController/PDBOverlayWidgetController.h"
#include "UI/WidgetController/PDBWidgetController.h"

UPDBOverlayWidgetController* APDBHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UPDBOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void APDBHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass 미설정 — BP_PDBHUD에서 지정 필요"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass 미설정 — BP_PDBHUD에서 지정 필요"));

	OverlayWidget = CreateWidget<UPDBUserWidget>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UPDBOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
