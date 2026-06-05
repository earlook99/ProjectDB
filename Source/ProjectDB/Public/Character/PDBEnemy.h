#pragma once

#include "CoreMinimal.h"
#include "Character/PDBCharacterBase.h"
#include "Interaction/PDBEnemyInterface.h"
#include "UI/WidgetController/PDBOverlayWidgetController.h"
#include "PDBEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class PROJECTDB_API APDBEnemy : public APDBCharacterBase, public IPDBEnemyInterface
{
	GENERATED_BODY()

public:
	APDBEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
