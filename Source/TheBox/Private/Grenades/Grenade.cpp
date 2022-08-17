#include "Grenades/Grenade.h"

// Sets default values
AGrenade::AGrenade() : CurrentNumGrenades(4), MaxNumGrenades(4)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Grenade Mesh"));
	GrenadeMesh->SetCastShadow(false);

	SetRootComponent(GrenadeMesh);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GrenadeMesh))
		GrenadeInstance = GrenadeMesh->GetAnimInstance();
}

