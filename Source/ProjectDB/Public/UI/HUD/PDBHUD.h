#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PDBHUD.generated.h"

class UPDBUserWidget;
class UPDBOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROJECTDB_API APDBHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPDBOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY()
	TObjectPtr<UPDBUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPDBUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UPDBOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPDBOverlayWidgetController> OverlayWidgetControllerClass;
};
