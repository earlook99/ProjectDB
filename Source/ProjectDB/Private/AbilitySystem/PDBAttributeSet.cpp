#include "AbilitySystem/PDBAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Interaction/PDBCombatInterface.h"

UPDBAttributeSet::UPDBAttributeSet()
{
}

void UPDBAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UPDBAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float CurrentDamage = GetDamage();
		SetDamage(0.f);
		SetHealth(FMath::Clamp(GetHealth() - CurrentDamage, 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Incoming Damage: %.1f, Health: %.1f"), CurrentDamage, GetHealth());
		
		if (GetHealth() <= 0.f)
		{
			IPDBCombatInterface* CombatInterface = Cast<IPDBCombatInterface>(Data.Target.AbilityActorInfo->AvatarActor.Get());
			if (!CombatInterface) return;
			
			CombatInterface->Die();
		}
	}
}

void UPDBAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Strength, OldStrength);
}

void UPDBAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Intelligence, OldIntelligence);
}

void UPDBAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Resilience, OldResilience);
}

void UPDBAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Vigor, OldVigor);
}

void UPDBAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, MaxHealth, OldMaxHealth);
}

void UPDBAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, MaxMana, OldMaxMana);
}

void UPDBAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Health, OldHealth);
}

void UPDBAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Mana, OldMana);
}
