#include "AbilitySystem/TargetType/PDBTargetType.h"

#include "Character/PDBCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UPDBTargetType::ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const
{

}

void UPDBTargetType::GatherOverlapCandidates(AActor* Instigator, const FVector& Center, float Radius, TArray<AActor*>& OutActors) const
{
	if (!Instigator) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Instigator);

	UKismetSystemLibrary::SphereOverlapActors(
		Instigator->GetWorld(),
		Center,
		Radius,
		ObjectTypes,
		APDBCharacterBase::StaticClass(),
		ActorsToIgnore,
		OutActors);
}
