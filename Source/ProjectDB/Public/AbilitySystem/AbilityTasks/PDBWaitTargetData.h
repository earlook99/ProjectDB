#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PDBWaitTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataReadyDelegate, const FGameplayAbilityTargetDataHandle&, Handle);

UENUM(BlueprintType)
enum class EPDBAcquireMode : uint8
{
	Cursor UMETA(DisplayName="Cursor"),
	Target UMETA(DisplayName="Target")
};

UCLASS()
class PROJECTDB_API UPDBWaitTargetData : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FTargetDataReadyDelegate ValidData;
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UPDBWaitTargetData* WaitTargetData(UGameplayAbility* OwningAbility, EPDBAcquireMode InAcquireMode);
	
	virtual void Activate() override;
	
private:
	EPDBAcquireMode AcquireMode;
	
	void GatherTargetData();
	void GatherCursorData();
	
	void SendHitResult(const FHitResult& Hit);
	void SendTargetDataHandle(const FGameplayAbilityTargetDataHandle& DataHandle);
	
	void SendEmptyData();

	void OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& Handle, FGameplayTag Tag);
};
