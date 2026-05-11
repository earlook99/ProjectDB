#pragma once

#include "CoreMinimal.h"
#include "Character/PDBCharacterBase.h"
#include "Interaction/PDBEnemyInterface.h"
#include "PDBEnemy.generated.h"

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
};
