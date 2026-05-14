#include "Actor/PDBEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameplayEffect.h"

APDBEffectActor::APDBEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APDBEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APDBEffectActor::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &APDBEffectActor::OnSphereEndOverlap);
}

FActiveGameplayEffectHandle APDBEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return FActiveGameplayEffectHandle();
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	const bool bShouldRemoveOnEnd = bIsInfinite && InfiniteEffectRemovalPolicy == EPDBEffectRemovalPolicy::RemoveOnEndOverlap;

	if (bShouldRemoveOnEnd)
	{
		ActiveGameplayEffects.Add(ActiveEffectHandle, TargetASC);
	}

	return ActiveEffectHandle;
}

void APDBEffectActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const
FHitResult& SweepResult)
{
	if (InstantGameplayEffectClass && InstantEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, InstantGameplayEffectClass);
	}
	if (DurationGameplayEffectClass && DurationEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, DurationGameplayEffectClass);
	}
	if (InfiniteGameplayEffectClass && InfiniteEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, InfiniteGameplayEffectClass);
	}
}

void APDBEffectActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InstantGameplayEffectClass && InstantEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, InstantGameplayEffectClass);
	}
	if (DurationGameplayEffectClass && DurationEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, DurationGameplayEffectClass);
	}
	if (InfiniteGameplayEffectClass && InfiniteEffectApplicationPolicy == EPDBEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, InfiniteGameplayEffectClass);
	}
	
	if (InfiniteEffectRemovalPolicy == EPDBEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC == nullptr) return;

		TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

		for (TPair<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent>>& Pair : ActiveGameplayEffects)
		{
			if (TargetASC == Pair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(Pair.Key, 1);
				ActiveEffectHandles.Add(Pair.Key);
			}
		}

		for (FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectHandles)
		{
			ActiveGameplayEffects.Remove(ActiveEffectHandle);
		}
	}
}
