#include "Interactables/Animated/AnimatedInteractables.h"
#include "Character/Player/PlayerCharacter.h"
#include "Pickups/PickupBase/PickupBase.h"
#include "Kismet/GameplayStatics.h"
#include <Structs/HexColors/Str_CustomHexColors.h>

AAnimatedInteractables::AAnimatedInteractables() : MaxNumToSpawn(0), PickupIndex(0), bIsWeaponContainer(false), bHasBeenOpned(false), CaseOpenTimer(0.6F)
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

	PickupEnum = static_cast<EPickupType>(FMath::RandRange(1, 5));

	if (PickupEnum == EPickupType::EPT_Weapon)
		MaxNumToSpawn = 15;

	else
		MaxNumToSpawn = 3;
	
	SpawnSetup();
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

void AAnimatedInteractables::SpawnSetup()
{
	PickupIndex = FMath::RandRange(0, MaxNumToSpawn);

	if (IsValid(SKBaseMesh))
		SpawnTransform = SKBaseMesh->GetSocketTransform(L"SpawnSocket");

	TempPickupPtr = GetWorld()->SpawnActorDeferred<APickupBase>(PickupToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	TempPickupPtr->SetPickupBaseType(PickupEnum);

	SetPickupData(PickupEnum);

	UGameplayStatics::FinishSpawningActor(TempPickupPtr, SpawnTransform);

	if (IsValid(TempPickupPtr))
		TempPickupPtr->SetActorHiddenInGame(true);
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

	CaseOpenTimer = 0.6F;

	if (IsValid(TempPickupPtr))
		TempPickupPtr->SetActorHiddenInGame(false);
}

void AAnimatedInteractables::SetPickupData(EPickupType InType)
{
	switch (InType)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		TempPickupPtr->SetWeaponPickupName(static_cast<EPickupWeaponType>(PickupIndex));

		break;

	case EPickupType::EPT_Health:

		TempPickupPtr->SetHealthPickupType(static_cast<EPickupHealthType>(PickupIndex));

		break;

	case EPickupType::EPT_Ammo:

		TempPickupPtr->SetAmmoPickupType(static_cast<EPickupAmmoType>(PickupIndex));

		break;

	case EPickupType::EPT_Armor:
		break;

	case EPickupType::EPT_Grenade:

		TempPickupPtr->SetGrenadePickupType(static_cast<EPickupGrenadeType>(PickupIndex));

		break;

	default:

		TempPickupPtr->SetHealthPickupType(EPickupHealthType::EPH_HealthLarge);

		break;
	}
}


