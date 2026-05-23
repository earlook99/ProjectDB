#include "AbilitySystem/Abilities/PDBActionAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/PDBGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AbilityTasks/PDBTargetDataUnderCursor.h"
#include "Character/PDBCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

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

	UPDBTargetDataUnderCursor* TargetTask = UPDBTargetDataUnderCursor::WaitTargetDataUnderCursor(this);
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
	
	SourceASC->ExecuteGameplayCue(FPDBGameplayTags::Get().GameplayCue_SoulSiphon_Cast);
	
	TArray<AActor*> TargetActors;
	GatherConeTargets(TargetActors);
	
	for (TSubclassOf<UGameplayEffect> EffectClass : Container->EffectClasses)
	{
		FGameplayEffectSpecHandle Handle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
		
		for (AActor* Actor : TargetActors)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
			if (!TargetASC) continue;
		
			SourceASC->ApplyGameplayEffectSpecToTarget(*Handle.Data.Get(), TargetASC);
			UE_LOG(LogTemp, Display, TEXT("Damaged!"));
		}
	}
}

void UPDBActionAbility::GatherConeTargets(TArray<AActor*>& OutActors) const
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	if (!SourceActor) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SourceActor);
	
	TArray<AActor*> OverlappedActors;
	
	UClass* ActorFilter = APDBCharacterBase::StaticClass();
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), 
		SourceActor->GetActorLocation(), 
		ConeRadius, 
		ObjectTypes,
		ActorFilter,
		ActorsToIgnore,
		OverlappedActors
		);
	
	for (AActor* Actor : OverlappedActors)
	{
		FVector Delta = Actor->GetActorLocation() - SourceActor->GetActorLocation();
		Delta.Z = 0.f;
		FVector Direction = Delta.GetSafeNormal();
		
		FVector Forward = SourceActor->GetActorForwardVector();
		Forward.Z = 0.f;
		FVector ForwardVector = Forward.GetSafeNormal();
		
		if (FMath::Cos(FMath::DegreesToRadians(ConeHalfAngle)) <= FVector::DotProduct(Direction, ForwardVector))
		{
			OutActors.Add(Actor);
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
	if (!TargetData) return;
	
	if (!TargetData->HasHitResult()) return;
	const FHitResult* HitResult = TargetData->GetHitResult();

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	FVector Direction = HitResult->ImpactPoint - Avatar->GetActorLocation();
	Direction.Z = 0.f;
	Avatar->SetActorRotation(Direction.GetSafeNormal().Rotation());
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "MontageTask", AbilityMontage, 1.5);
	MontageTask->OnCompleted.AddDynamic(this, &UPDBActionAbility::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UPDBActionAbility::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UPDBActionAbility::OnMontageCancelled);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.Montage.Hit")));
	EventTask->EventReceived.AddDynamic(this, &UPDBActionAbility::OnGameplayEventReceived);
	EventTask->ReadyForActivation();
}
