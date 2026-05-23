#include "AbilitySystem/AbilityTasks/PDBTargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"


UPDBTargetDataUnderCursor* UPDBTargetDataUnderCursor::WaitTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
	return UAbilityTask::NewAbilityTask<UPDBTargetDataUnderCursor>(OwningAbility);
}

void UPDBTargetDataUnderCursor::Activate()
{
	Super::Activate();
	
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendCursorData();
	}
	else
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(), 
			GetActivationPredictionKey()
			).AddUObject(this, &UPDBTargetDataUnderCursor::OnTargetDataReplicated);
		
		if (!AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			GetAbilitySpecHandle(), 
			GetActivationPredictionKey()))
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UPDBTargetDataUnderCursor::SendCursorData()
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return;

	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(CursorHit));
	
	FScopedPredictionWindow Window(AbilitySystemComponent.Get());
	
	AbilitySystemComponent->CallServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		TargetDataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey
		);
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
		EndTask();
	}
}

void UPDBTargetDataUnderCursor::OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& Handle, FGameplayTag Tag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Handle);
		EndTask();
	}
}
