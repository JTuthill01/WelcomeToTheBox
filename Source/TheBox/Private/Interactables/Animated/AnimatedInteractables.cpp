#include "Interactables/Animated/AnimatedInteractables.h"

AAnimatedInteractables::AAnimatedInteractables() : bHasBeenOpned(false)
{
	PrimaryActorTick.bCanEverTick = false;

	SKBaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton Base Mesh"));
	SKBaseMesh->SetupAttachment(BaseRootComponent);
	SKBaseMesh->SetCastShadow(false);
	SKBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SKBaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SKBaseMesh->SetGenerateOverlapEvents(true);

	WidgetText = FText::FromString(TEXT("Press E To Open"));
}

void AAnimatedInteractables::BeginPlay()
{
	Super::BeginPlay();
}

void AAnimatedInteractables::InteractableFound_Implementation()
{
	if (bHasBeenOpned == true)
		return;

	Super::InteractableFound_Implementation();
}

void AAnimatedInteractables::InteractWithObject_Implementation()
{
	if (bHasBeenOpned == true)
		return;

	Super::InteractWithObject_Implementation();

	Open();
}

void AAnimatedInteractables::Open()
{
	SKBaseMesh->PlayAnimation(AnimToPlay, false);

	bHasBeenOpned = true;
}
