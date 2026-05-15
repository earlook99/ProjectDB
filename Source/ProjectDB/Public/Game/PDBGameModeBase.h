#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PDBGameModeBase.generated.h"

class UPDBCharacterClassInfo;

UCLASS()
class PROJECTDB_API APDBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UPDBCharacterClassInfo> CharacterClassInfo;
};
