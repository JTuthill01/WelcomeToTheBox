#include "Interactables/Animated/AnimatedInteractables.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Character/Player/PlayerCharacter.h"

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

	FActorSpawnParameters Parms;
	Parms.Owner = this;
	Parms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	/* Temp code for weapon pickup spawning. IT ACTUALLY FUCKING WORKS!!!!!!!*/

	FVector Loc{ SKBaseMesh->GetSocketLocation("SpawnSocket") };

	FRotator Rot{ SKBaseMesh->GetSocketRotation("SpawnSocket") };

	TObjectPtr<AWeaponBase> Temp = GetWorld()->SpawnActor<AWeaponBase>(PlayerRef->GetWeaponSlotArray()[0]->GetSubClass(), Loc, Rot, Parms);
}
