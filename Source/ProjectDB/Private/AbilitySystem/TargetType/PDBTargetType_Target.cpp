#include "AbilitySystem/TargetType/PDBTargetType_Target.h"

void UPDBTargetType_Target::ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const
{
	Super::ResolveTargets(Instigator, CursorHit, OutTargets);
	
	AActor* TargetActor = CursorHit.GetActor();
	
	if (!TargetActor) return;
	if (TargetActor == Instigator) return;
	
	OutTargets.Add(CursorHit.GetActor());
}
