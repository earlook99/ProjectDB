#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PDBDamageTextComponent.generated.h"

UCLASS()
class PROJECTDB_API UPDBDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
};
