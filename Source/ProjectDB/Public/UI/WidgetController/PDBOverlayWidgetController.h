#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PDBWidgetController.h"
#include "PDBOverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class PROJECTDB_API UPDBOverlayWidgetController : public UPDBWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
};
