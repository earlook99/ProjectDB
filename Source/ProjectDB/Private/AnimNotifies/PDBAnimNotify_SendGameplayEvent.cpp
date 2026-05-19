#include "AnimNotifies/PDBAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/PDBCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UPDBAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!EventTag.IsValid()) return;
	if (!MeshComp) return;
		
	APDBCharacter* OwnerCharacter = Cast<APDBCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;
	
	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = OwnerCharacter;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacter, EventTag, Payload);
}

#if WITH_EDITOR
FString UPDBAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}
#endif
