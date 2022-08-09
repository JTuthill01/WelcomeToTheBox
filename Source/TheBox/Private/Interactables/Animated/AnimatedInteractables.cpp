#include "Interactables/Animated/AnimatedInteractables.h"
#include "Character/Player/PlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Pickups/PickupBase/PickupBase.h"

AAnimatedInteractables::AAnimatedInteractables() : bHasBeenOpned(false), CaseOpenTimer(0.4F)
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

	GetWorldTimerManager().SetTimer(CaseOpenTimerHandle, this, &AAnimatedInteractables::Spawn, CaseOpenTimer, false);
}

void AAnimatedInteractables::Spawn()
{
	GetWorldTimerManager().ClearTimer(CaseOpenTimerHandle);

	CaseOpenTimer = 0.4F;

	FTransform FXTransform = SKBaseMesh->GetSocketTransform(L"SpawnSocket");

	FQuat FXQuat = FXTransform.GetRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CaseOpenFX, FXTransform.GetTranslation(), FXQuat.Rotator());

	GetWorld()->SpawnActor<APickupBase>(PickupToSpawn, FXTransform);
}
