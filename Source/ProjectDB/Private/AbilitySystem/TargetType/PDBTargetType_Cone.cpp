#include "AbilitySystem/TargetType/PDBTargetType_Cone.h"

#include "GameFramework/Actor.h"

void UPDBTargetType_Cone::ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const
{
	TArray<AActor*> OverlappedActors;
	GatherOverlapCandidates(Instigator, Instigator->GetActorLocation(), ConeRadius, OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		FVector Delta = Actor->GetActorLocation() - Instigator->GetActorLocation();
		Delta.Z = 0.f;
		FVector Direction = Delta.GetSafeNormal();

		FVector Forward = Instigator->GetActorForwardVector();
		Forward.Z = 0.f;
		FVector ForwardVector = Forward.GetSafeNormal();

		if (FMath::Cos(FMath::DegreesToRadians(ConeHalfAngle)) <= FVector::DotProduct(Direction, ForwardVector))
		{
			OutTargets.Add(Actor);
		}
	}
}
