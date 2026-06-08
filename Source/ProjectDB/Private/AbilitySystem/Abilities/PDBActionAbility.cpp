#include "AbilitySystem/Abilities/PDBActionAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/PDBGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AbilityTasks/PDBWaitTargetData.h"
#include "AbilitySystem/TargetType/PDBTargetType.h"
#include "Character/PDBCharacterBase.h"

UPDBActionAbility::UPDBActionAbility()
{
	ActivationOwnedTags.AddTag(FPDBGameplayTags::Get().State_Block_Movement);
}

void UPDBActionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UPDBWaitTargetData* TargetTask = UPDBWaitTargetData::WaitTargetData(this, AcquireMode);
	TargetTask->ValidData.AddDynamic(this, &UPDBActionAbility::OnTargetDataReady);
	TargetTask->ReadyForActivation();
}

void UPDBActionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPDBActionAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	if (!HasAuthority(&CurrentActivationInfo)) return;
	
	FPDBEffectContainer* Container = EffectContainerMap.Find(Payload.EventTag);
	if (!Container) return;
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;
	
	UPDBTargetType* TargetType = Container->TargetType;
	if (!TargetType) return;
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;
	
	FGameplayCueParameters Params;
	
	switch (CueOrigin)
	{
	case EPDBCueOrigin::Caster:
		Params.Location = Avatar->GetActorLocation();
		break;
		
	case EPDBCueOrigin::Cursor:
		Params.Location = CursorHit.ImpactPoint;
		break;
		
	case EPDBCueOrigin::Custom:
		Params.Location = GetCustomCueLocation();
		break;
		
	default:
		break;
	}
	
	SourceASC->ExecuteGameplayCue(CastCueTag, Params);
	
	TArray<AActor*> TargetActors;
	TargetType->ResolveTargets(Avatar, CursorHit, TargetActors);
	
	for (TSubclassOf<UGameplayEffect> EffectClass : Container->EffectClasses)
	{
		FGameplayEffectSpecHandle Handle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
		Handle.Data->SetSetByCallerMagnitude(FPDBGameplayTags::Get().Data_Damage, Container->Damage.GetValueAtLevel(GetAbilityLevel()));

		for (AActor* Actor : TargetActors)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			if (!TargetASC) continue;
		
			SourceASC->ApplyGameplayEffectSpecToTarget(*Handle.Data.Get(), TargetASC);
			UE_LOG(LogTemp, Display, TEXT("Damaged!"));
		}
	}
}

void UPDBActionAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPDBActionAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPDBActionAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPDBActionAbility::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(0);
	if (!TargetData || !TargetData->HasHitResult())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	const FHitResult* HitResult = TargetData->GetHitResult();
	CursorHit = *HitResult;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	FVector Direction = HitResult->ImpactPoint - Avatar->GetActorLocation();
	Direction.Z = 0.f;
	Avatar->SetActorRotation(Direction.GetSafeNormal().Rotation());
	
	ReceivePerformAbility();
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "MontageTask", AbilityMontage, 1.f);
	MontageTask->OnCompleted.AddDynamic(this, &UPDBActionAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UPDBActionAbility::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UPDBActionAbility::OnMontageCancelled);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.Montage.Hit")), nullptr, 
	true);
	EventTask->EventReceived.AddDynamic(this, &UPDBActionAbility::OnGameplayEventReceived);
	EventTask->ReadyForActivation();
}
