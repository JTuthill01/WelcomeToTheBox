#include "Interactables/InteractableBase.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Interfaces/Controller/PlayerControllerInterface.h"
#include "Widgets/InteractableFound/InteractableFoundWidget.h"
#include "Character/PlayerController/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// Sets default values
AInteractableBase::AInteractableBase() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Base Root Component"));
	SetRootComponent(BaseRootComponent);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(PlayerRef))
		PlayerRef->Clear.AddDynamic(this, &AInteractableBase::OnClearViewport);
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::InteractableFound_Implementation()
{
	if (!IsValid(InteractableFoundWidget))
	{
		InteractableFoundWidget = CreateWidget<UInteractableFoundWidget>(GetWorld(), InteractableWidgetSub);

		if (IsValid(InteractableFoundWidget))
		{
			InteractableFoundWidget->InteractText->SetText(WidgetText);
			InteractableFoundWidget->InteractableIcon->SetBrushFromTexture(IconTexture);
			InteractableFoundWidget->AddToViewport(-1);
		}
	}

	else if (!InteractableFoundWidget->IsInViewport())
		InteractableFoundWidget->AddToViewport(-1);
}

void AInteractableBase::InteractWithObject_Implementation() {}

void AInteractableBase::OnClearViewport()
{
	if (IsValid(InteractableFoundWidget))
		if (InteractableFoundWidget->IsInViewport())
			InteractableFoundWidget->RemoveFromViewport();
}

