#include "AbilitySystem/AbilityTasks/PDBWaitTargetData.h"
#include "AbilitySystemComponent.h"
#include "Player/PDBPlayerController.h"


UPDBWaitTargetData* UPDBWaitTargetData::WaitTargetData(UGameplayAbility* OwningAbility, EPDBAcquireMode InAcquireMode)
{
	UPDBWaitTargetData* TargetData =  UAbilityTask::NewAbilityTask<UPDBWaitTargetData>(OwningAbility);
	TargetData->AcquireMode = InAcquireMode;
	return TargetData;
}

void UPDBWaitTargetData::Activate()
{
	Super::Activate();
	
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		if (AcquireMode == EPDBAcquireMode::Cursor)
		{
			GatherCursorData();
		}
		else if (AcquireMode == EPDBAcquireMode::Target)
		{
			GatherTargetData();
		}
	}
	else
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(), 
			GetActivationPredictionKey()
			).AddUObject(this, &UPDBWaitTargetData::OnTargetDataReplicated);
		
		if (!AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			GetAbilitySpecHandle(), 
			GetActivationPredictionKey()))
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UPDBWaitTargetData::GatherTargetData()
{
	APDBPlayerController* PC = Cast<APDBPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	if (!PC)
	{
		SendEmptyData();
		return;
	}
	
	TWeakObjectPtr<AActor> TargetActor = PC->GetTargetActor();
	if (!TargetActor.IsValid())
	{
		SendEmptyData();
		return;
	}
	
	FHitResult TargetHit;
	TargetHit.HitObjectHandle = FActorInstanceHandle(TargetActor.Get());
	TargetHit.ImpactPoint = TargetActor->GetActorLocation();
	TargetHit.bBlockingHit = true;
	
	SendHitResult(TargetHit);
}

void UPDBWaitTargetData::GatherCursorData()
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC)
	{
		SendEmptyData();
		return;
	}

	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		SendEmptyData();
		return;
	}

	SendHitResult(CursorHit);
}

void UPDBWaitTargetData::SendHitResult(const FHitResult& Hit)
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));
	
	SendTargetDataHandle(TargetDataHandle);
}

void UPDBWaitTargetData::SendTargetDataHandle(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	FScopedPredictionWindow Window(AbilitySystemComponent.Get());
	
	AbilitySystemComponent->CallServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey
		);
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
		EndTask();
	}
}

void UPDBWaitTargetData::SendEmptyData()
{
	SendTargetDataHandle(FGameplayAbilityTargetDataHandle());
}


void UPDBWaitTargetData::OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& Handle, FGameplayTag Tag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Handle);
		EndTask();
	}
}
