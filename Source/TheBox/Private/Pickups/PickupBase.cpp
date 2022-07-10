#include "Pickups/PickupBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Interfaces/Pickup/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "EditorAssetLibrary.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCastShadow(false);
	BaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	Setup();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APickupBase::InteractableFound_Implementation()
{
}

void APickupBase::InteractWithObject_Implementation()
{
	switch (PickupBaseType)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:
		break;

	case EPickupType::EPT_Health:

		HealthPickup();

		OnClearViewport();

		break;

	case EPickupType::EPT_Ammo:
		break;

	case EPickupType::EPT_Armor:
		break;

	default:
		break;
	}
}

void APickupBase::OnClearViewport()
{
	
}

void APickupBase::Setup()
{
	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (IsValid(PlayerRef))
		PlayerRef->Clear.AddDynamic(this, &APickupBase::OnClearViewport);

	else
		return;
}

void APickupBase::HealthPickup()
{
	switch (BaseHealthType)
	{
	case EPickupHealthType::EPH_None:
		break;

	case EPickupHealthType::EPH_HealthSmall:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), HealthValue, NULL);

			Destroy();
		}

		break;

	case EPickupHealthType::EPH_HealthMedium:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), HealthValue, NULL);

			Destroy();
		}

		break;

	case EPickupHealthType::EPH_HealthLarge:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), HealthValue, NULL);

			Destroy();
		}

		break;

	default:
		break;
	}
}
