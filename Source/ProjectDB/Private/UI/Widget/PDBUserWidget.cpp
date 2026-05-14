#include "UI/Widget/PDBUserWidget.h"

void UPDBUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
