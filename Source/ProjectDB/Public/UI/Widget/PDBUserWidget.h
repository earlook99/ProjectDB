#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDBUserWidget.generated.h"

UCLASS()
class PROJECTDB_API UPDBUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
