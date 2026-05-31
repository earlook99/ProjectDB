#include "AbilitySystem/GameplayEffectComponents/PDBDisplacementGameplayEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/PDBAbilitySystemLibrary.h"

void UPDBDisplacementGameplayEffectComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);
	
	if (!ActiveGEContainer.OwnerIsNetAuthority) return;
	
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceAvatar = GESpec.GetContext().GetInstigator();
	
	const FVector Force = UPDBAbilitySystemLibrary::ComputeKnockback(SourceAvatar, TargetAvatar, KnockbackMagnitude);
	if (ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatar))
	{
		TargetCharacter->LaunchCharacter(Force, true, true);
	}
}
