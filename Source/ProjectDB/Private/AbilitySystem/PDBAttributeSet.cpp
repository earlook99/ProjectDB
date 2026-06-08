#include "AbilitySystem/PDBAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/PDBGameplayEffectContext.h"
#include "GameFramework/Character.h"
#include "Interaction/PDBCombatInterface.h"
#include "Player/PDBPlayerController.h"

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

	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPDBAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
}

void UPDBAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
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
		
		bool bCriticalHit = false;
		
		FPDBGameplayEffectContext* Context = static_cast<FPDBGameplayEffectContext*>(Props.ContextHandle.Get());
		if (Context)
		{
			bCriticalHit = Context->IsCriticalHit();
		}
		ShowFloatingText(Props, CurrentDamage, false, bCriticalHit);

		if (GetHealth() <= 0.f)
		{
			IPDBCombatInterface* CombatInterface = Cast<IPDBCombatInterface>(Props.TargetAvatarActor);
			if (!CombatInterface) return;
			
			CombatInterface->Die();
		}
	}
}

void UPDBAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProperties) const
{
	OutProperties.ContextHandle = Data.EffectSpec.GetContext();
	OutProperties.SourceASC = OutProperties.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(OutProperties.SourceASC) && OutProperties.SourceASC->AbilityActorInfo.IsValid() && OutProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProperties.SourceAvatarActor = OutProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		OutProperties.SourceController = OutProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (OutProperties.SourceController == nullptr && OutProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(OutProperties.SourceAvatarActor))
			{
				OutProperties.SourceController = Pawn->GetController();
			}
		}
		OutProperties.SourceCharacter = Cast<ACharacter>(OutProperties.SourceAvatarActor);
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProperties.TargetASC = &Data.Target;
		OutProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		OutProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		if (OutProperties.TargetController == nullptr && OutProperties.TargetAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(OutProperties.TargetAvatarActor))
			{
				OutProperties.TargetController = Pawn->GetController();
			}
		}
		OutProperties.TargetCharacter = Cast<ACharacter>(OutProperties.TargetAvatarActor);
	}
}

void UPDBAttributeSet::ShowFloatingText(const FEffectProperties& Props, float DamageAmount, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.TargetCharacter == Props.SourceCharacter) return;

	APDBPlayerController* SourcePC = Cast<APDBPlayerController>(Props.SourceController);
	if (SourcePC)
	{
		SourcePC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bCriticalHit);
	}

	APDBPlayerController* TargetPC = Cast<APDBPlayerController>(Props.TargetController);
	if (TargetPC)
	{
		TargetPC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bCriticalHit);
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

void UPDBAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, Armor, OldArmor);
}

void UPDBAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UPDBAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPDBAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

