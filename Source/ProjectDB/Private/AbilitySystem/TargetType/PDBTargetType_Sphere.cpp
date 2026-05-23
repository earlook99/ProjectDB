#include "AbilitySystem/TargetType/PDBTargetType_Sphere.h"

#include "Character/PDBCharacterBase.h"
#include "Components/CapsuleComponent.h"

void UPDBTargetType_Sphere::ResolveTargets(AActor* Instigator, const FHitResult& CursorHit, TArray<AActor*>& OutTargets) const
{
	FVector ToCursor = CursorHit.ImpactPoint - Instigator->GetActorLocation();
	ToCursor.Z = 0.f;
	FVector ClampedToCursor = ToCursor.GetClampedToMaxSize(CastRange);
	FVector Center = Instigator->GetActorLocation() + ClampedToCursor;

	TArray<AActor*> OverlappedActors;
	GatherOverlapCandidates(Instigator, Center, CircleRadius + VerticalTolerance, OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		APDBCharacterBase* Character = Cast<APDBCharacterBase>(Actor);
		if (!Character) continue;

		float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (FVector::DistSquared2D(Actor->GetActorLocation(), Center) <= FMath::Square(CircleRadius + CapsuleRadius))
		{
			OutTargets.Add(Actor);
		}
	}
}
