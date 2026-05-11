#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDBCharacterBase.generated.h"

UCLASS(Abstract)
class PROJECTDB_API APDBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APDBCharacterBase();

protected:
	virtual void BeginPlay() override;
};
