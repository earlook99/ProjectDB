#pragma once

#include "CoreMinimal.h"
#include "Character/PDBCharacterBase.h"
#include "PDBCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTDB_API APDBCharacter : public APDBCharacterBase
{
	GENERATED_BODY()

public:
	APDBCharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> TopDownCamera;
};
