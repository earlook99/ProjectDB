#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PDBEnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPDBEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTDB_API IPDBEnemyInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
