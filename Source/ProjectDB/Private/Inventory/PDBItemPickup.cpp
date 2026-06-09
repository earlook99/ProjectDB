#include "Inventory/PDBItemPickup.h"

#include "Components/SphereComponent.h"
#include "Player/PDBPlayerState.h"
#include "Inventory/PDBInventoryComponent.h"
#include "GameFramework/Pawn.h"

APDBItemPickup::APDBItemPickup()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bReplicates = true;
}

void APDBItemPickup::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APDBItemPickup::OnSphereOverlap);
}

void APDBItemPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;
	
	APDBPlayerState* PS = Pawn->GetPlayerState<APDBPlayerState>();
	if (!PS) return;
	
	UPDBInventoryComponent *Inv = PS->GetInventoryComponent();
	if (!Inv) return;
	
	Inv->AddItem(ItemDef, Quantity);
	
	Destroy();
}
