#include "Interactables/InteractableDoors/InteractableDoor.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Interfaces/Controller/PlayerControllerInterface.h"
#include "Widgets/InteractableFound/InteractableFoundWidget.h"
#include "Character/PlayerController/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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

