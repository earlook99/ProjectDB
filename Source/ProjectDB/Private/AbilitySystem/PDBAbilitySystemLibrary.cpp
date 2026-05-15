#include "AbilitySystem/PDBAbilitySystemLibrary.h"

#include "AbilitySystem/Data/PDBCharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "Game/PDBGameModeBase.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	void ApplyEffectToSelf(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
	{
		if (!GameplayEffectClass)
		{
			return;
		}

		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(ASC->GetAvatarActor());

		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UPDBCharacterClassInfo* UPDBAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const APDBGameModeBase* GameMode = Cast<APDBGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)
	{
		return nullptr;
	}
	return GameMode->CharacterClassInfo;
}

void UPDBAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UPDBCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr || ASC == nullptr)
	{
		return;
	}
	
	ApplyEffectToSelf(ASC, CharacterClassInfo->PrimaryAttributes, 1.f);
	ApplyEffectToSelf(ASC, CharacterClassInfo->SecondaryAttributes, 1.f);
	ApplyEffectToSelf(ASC, CharacterClassInfo->VitalAttributes, 1.f);
}
