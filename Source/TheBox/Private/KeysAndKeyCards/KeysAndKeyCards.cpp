#include "KeysAndKeyCards/KeysAndKeyCards.h"

// Sets default values
AKeysAndKeyCards::AKeysAndKeyCards()
{
	PrimaryActorTick.bCanEverTick = false;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Mesh"));
	SetRootComponent(KeyMesh);
}

// Called when the game starts or when spawned
void AKeysAndKeyCards::BeginPlay()
{
	Super::BeginPlay();
}

void AKeysAndKeyCards::InteractWithObject_Implementation()
{
	
}

void AKeysAndKeyCards::InteractableFound_Implementation()
{
	
}

void AKeysAndKeyCards::ClearViewport_Implementation() {}

