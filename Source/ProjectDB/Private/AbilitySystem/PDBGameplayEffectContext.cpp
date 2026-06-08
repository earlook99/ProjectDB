#include "AbilitySystem/PDBGameplayEffectContext.h"

bool FPDBGameplayEffectContext::IsCriticalHit() const
{
	return bIsCriticalHit;
}

void FPDBGameplayEffectContext::SetIsCriticalHit(bool bInIsCriticalHit)
{
	bIsCriticalHit = bInIsCriticalHit;
}

UScriptStruct* FPDBGameplayEffectContext::GetScriptStruct() const
{
	return FPDBGameplayEffectContext::StaticStruct();
}

FGameplayEffectContext* FPDBGameplayEffectContext::Duplicate() const
{
	FPDBGameplayEffectContext* NewContext = new FPDBGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

bool FPDBGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	uint8 RepBits = 0;
	if (Ar.IsSaving() && bIsCriticalHit)
	{
		RepBits |= 1 << 0;
	}
	Ar.SerializeBits(&RepBits, 1);
	if (Ar.IsLoading())
	{
		bIsCriticalHit = (RepBits & (1 << 0)) != 0;
	}

	return true;
}
