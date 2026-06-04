#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PDBCombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPDBCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTDB_API IPDBCombatInterface
{
	GENERATED_BODY()

public:
	virtual void Die() = 0;
};
