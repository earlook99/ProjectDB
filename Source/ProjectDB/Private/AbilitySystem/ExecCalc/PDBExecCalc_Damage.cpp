#include "AbilitySystem/ExecCalc/PDBExecCalc_Damage.h"

#include "AbilitySystem/PDBAttributeSet.h"
#include "AbilitySystem/PDBGameplayEffectContext.h"
#include "GameplayTags/PDBGameplayTags.h"

static constexpr float ArmorNormalizer = 100.f;
static constexpr float CriticalHitMultiplier = 2.f;

struct FPDBDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

	FPDBDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPDBAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPDBAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPDBAttributeSet, CriticalHitChance, Source, false);
	}
};

static const FPDBDamageStatics& DamageStatics()
{
	static FPDBDamageStatics DStatics;
	return DStatics;
}

UPDBExecCalc_Damage::UPDBExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
}

void UPDBExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
 Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle Handle = Spec.GetContext();
	
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Damage = Spec.GetSetByCallerMagnitude(FPDBGameplayTags::Get().Data_Damage);
	
	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalParams, Armor);
	
	float ArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvalParams, ArmorPenetration);
	
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvalParams, CriticalHitChance);
	
	float EffectiveArmor = Armor * (1.f - ArmorPenetration / 100.f);
	EffectiveArmor = FMath::Max(0.f, EffectiveArmor);
	
	Damage *= ArmorNormalizer / (ArmorNormalizer + EffectiveArmor);
	
	const bool bCriticalHit = FMath::RandRange(0.f, 100.f) <= CriticalHitChance;
	if (bCriticalHit)
	{
		Damage *= CriticalHitMultiplier;
		
		FPDBGameplayEffectContext* ContextHandle = static_cast<FPDBGameplayEffectContext*>(Handle.Get());
		if (ContextHandle)
		{
			ContextHandle->SetIsCriticalHit(true);
		}
	}
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPDBAttributeSet::GetDamageAttribute(), EGameplayModOp::Override, Damage));
}
