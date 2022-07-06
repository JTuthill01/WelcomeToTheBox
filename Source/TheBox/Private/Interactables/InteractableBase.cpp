#include "Interactables/InteractableBase.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Interfaces/Controller/PlayerControllerInterface.h"
#include "Widgets/InteractableFound/InteractableFoundWidget.h"
#include "Character/PlayerController/PlayerCharacterController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableBase::AInteractableBase() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Base Root Component"));
	SetRootComponent(BaseRootComponent);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	PC = IPlayerControllerInterface::Execute_SetControllerRef(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::InteractableFound_Implementation()
{
	if (!IsValid(PC))
		return;

	if (!IsValid(InteractableFoundWidget))
	{
		if (IsValid(InteractableFoundWidget))
		{
			InteractableFoundWidget->InteractText->SetText(WidgetText);
			InteractableFoundWidget->InteractableIcon->SetBrushFromTexture(IconTexture);
			InteractableFoundWidget->AddToViewport();
		}
	}

	else if (IsValid(InteractableFoundWidget))
		if (InteractableFoundWidget->IsInViewport())
			InteractableFoundWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AInteractableBase::InteractWithObject_Implementation()
{
	if (IsValid(InteractableFoundWidget))
		if (InteractableFoundWidget->IsInViewport())
			InteractableFoundWidget->SetVisibility(ESlateVisibility::Collapsed);
}

