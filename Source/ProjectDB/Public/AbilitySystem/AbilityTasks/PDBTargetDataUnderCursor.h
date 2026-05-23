#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PDBTargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataReadyDelegate, const FGameplayAbilityTargetDataHandle&, Handle);

UCLASS()
class PROJECTDB_API UPDBTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FTargetDataReadyDelegate ValidData;
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UPDBTargetDataUnderCursor* WaitTargetDataUnderCursor(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	
private:
	void SendCursorData();
	
	void OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& Handle, FGameplayTag Tag);
};
