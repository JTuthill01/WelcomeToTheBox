#include "Interactables/InteractableDoors/InteractableDoor.h"

AInteractableDoor::AInteractableDoor()
{
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorframe Mesh"));
	DoorFrameMesh->SetupAttachment(BaseRootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableDoor::InteractableFound_Implementation()
{
	Super::InteractableFound_Implementation();
}

void AInteractableDoor::InteractWithObject_Implementation()
{
	Super::InteractWithObject_Implementation();
}

