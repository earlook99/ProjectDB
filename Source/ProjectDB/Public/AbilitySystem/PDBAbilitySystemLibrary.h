#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PDBAbilitySystemLibrary.generated.h"

class UPDBCharacterClassInfo;
class UAbilitySystemComponent;
class AActor;

UCLASS()
class PROJECTDB_API UPDBAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "PDBAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
	static UPDBCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "PDBAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintPure, Category = "PDBAbilitySystemLibrary|Combat")
	static FVector ComputeKnockback(const AActor* Source, const AActor* Target, float Magnitude);
};
