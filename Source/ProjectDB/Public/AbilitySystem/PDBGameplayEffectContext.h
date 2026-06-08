#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "PDBGameplayEffectContext.generated.h"

USTRUCT()
struct PROJECTDB_API FPDBGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool bIsCriticalHit = false;
	
	bool IsCriticalHit() const;
	void SetIsCriticalHit(bool bInIsCriticalHit);
	
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FGameplayEffectContext* Duplicate() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
};

template<>
struct TStructOpsTypeTraits<FPDBGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPDBGameplayEffectContext>
{
    enum { WithNetSerializer = true, WithCopy = true };
};