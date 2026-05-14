#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PDBWidgetController.generated.h"

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS)
		: PlayerController(InPC), PlayerState(InPS), AbilitySystemComponent(InASC), AttributeSet(InAS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class PROJECTDB_API UPDBWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
