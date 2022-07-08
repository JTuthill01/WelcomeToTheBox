#include "Interactables/Containers/InteractableContainers.h"

AInteractableContainers::AInteractableContainers()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractableContainers::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableContainers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableContainers::InteractableFound_Implementation()
{
	Super::InteractableFound_Implementation();
}

void AInteractableContainers::InteractWithObject_Implementation()
{
	Super::InteractWithObject_Implementation();
}
