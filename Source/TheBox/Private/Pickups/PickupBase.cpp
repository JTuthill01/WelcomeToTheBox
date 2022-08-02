#include "Pickups/PickupBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "JsonComponents/PickupComponent/PickupComponent.h"
#include "Interfaces/Pickup/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponBase/WeaponBase.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCastShadow(false);
	BaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	PickupParser = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup Parser"));
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

	switch (PickupBaseType)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		SetWeaponData(PickupWeaponName);

		break;

	case EPickupType::EPT_Health:

		SetHealthData(BaseHealthType);
		
		break;

	case EPickupType::EPT_Ammo:

		SetAmmoData(PickupAmmoType);

		break;

	case EPickupType::EPT_Armor:

		SetArmorData();

		break;

	default:
		break;
	}
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

		HealthPickup(BaseHealthType);

		OnClearViewport();

		break;

	case EPickupType::EPT_Ammo:

		AmmoPickup(PickupAmmoType);

		break;

	case EPickupType::EPT_Armor:

		if (!PlayerRef->GetHealthComponent()->HasFullArmor())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), NULL, PickupData.ArmorValue);

			Destroy();
		}

		OnClearViewport();

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

void APickupBase::HealthPickup(EPickupHealthType InHealthType)
{
	switch (InHealthType)
	{
	case EPickupHealthType::EPH_None:
		break;

	case EPickupHealthType::EPH_HealthSmall:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), PickupData.HealthPackValue, NULL);

			Destroy();
		}

		break;

	case EPickupHealthType::EPH_HealthMedium:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), PickupData.HealthPackValue, NULL);

			Destroy();
		}

		break;

	case EPickupHealthType::EPH_HealthLarge:

		if (!PlayerRef->GetHealthComponent()->HasFullHealth())
		{
			IPickupInterface::Execute_UpdatePlayerStats(PlayerRef->GetHealthComponent(), PickupData.HealthPackValue, NULL);

			Destroy();
		}

		break;

	default:
		break;
	}
}

void APickupBase::AmmoPickup(EPickupAmmoType InAmmoType)
{
	switch (InAmmoType)
	{
	case EPickupAmmoType::EPA_None:
		break;

	case EPickupAmmoType::EPH_PistolAmmo:

		if (PlayerRef->GetCurrentWeapon()->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			if (!PlayerRef->GetCurrentWeapon()->IsAmmoFull())
			{
				PlayerRef->GetCurrentWeapon()->SetTotalAmmo(PickupData.PistolAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPS_RifleAmmo:

		if (PlayerRef->GetCurrentWeapon()->GetWeaponType() == EWeaponType::EWT_Rifle)
		{
			if (!PlayerRef->GetCurrentWeapon()->IsAmmoFull())
			{
				PlayerRef->GetCurrentWeapon()->SetTotalAmmo(PickupData.RifleAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPA_ShotgunAmmo:

		if (PlayerRef->GetCurrentWeapon()->GetWeaponType() == EWeaponType::EWT_Shotgun)
		{
			if (!PlayerRef->GetCurrentWeapon()->IsAmmoFull())
			{
				PlayerRef->GetCurrentWeapon()->SetTotalAmmo(PickupData.ShotgunAmmoValue);

				Destroy();
			}
		}

		break;

	default:
		break;
	}
}

void APickupBase::SetData()
{
	TObjectPtr<UStaticMesh> NewMesh = LoadObject<UStaticMesh>(this, *PickupParser->MeshFilePathString);

	TObjectPtr<UMaterialInstance> NewInstance = LoadObject<UMaterialInstance>(this, *PickupParser->IconFilePathString);

	if (IsValid(NewMesh))
		BaseMesh->SetStaticMesh(NewMesh);

	PickupData.PickupName = FName(*PickupParser->PickupNameString);
	PickupData.Icon = NewInstance;
	PickupData.PickupWidgetText = FText::FromString(PickupParser->WidgetTextString);
	PickupData.PickupType = static_cast<EPickupType>(PickupParser->PType);
	PickupData.PickupHealthType = static_cast<EPickupHealthType>(PickupParser->PHealthType);
	PickupData.PickupAmmoType = static_cast<EPickupAmmoType>(PickupParser->PAmmoType);
	PickupData.HealthPackValue = PickupParser->HealthValue;
	PickupData.ArmorValue = PickupParser->ArmValue;
	PickupData.PistolAmmoValue = PickupParser->PAmmoValue;
	PickupData.RifleAmmoValue = PickupParser->RAmmoValue;
	PickupData.ShotgunAmmoValue = PickupParser->SAmmoValue;
}

void APickupBase::SetWeaponPickupData()
{
	TObjectPtr<UStaticMesh> NewMesh = LoadObject<UStaticMesh>(this, *PickupParser->MeshFilePathString);

	TObjectPtr<UMaterialInstance> NewInstance = LoadObject<UMaterialInstance>(this, *PickupParser->IconFilePathString);

	if (IsValid(NewMesh))
		BaseMesh->SetStaticMesh(NewMesh);

	WeaponPickupStr.Icon = NewInstance;
	WeaponPickupStr.PickupType = static_cast<EPickupType>(PickupParser->PType);
	WeaponPickupStr.WeaponPickupName = FName(*PickupParser->PickupNameString);
	WeaponPickupStr.PickupWidgetText = PickupParser->WidgetTextString;
}

void APickupBase::SetHealthData(EPickupHealthType Health)
{
	switch (Health)
	{
	case EPickupHealthType::EPH_None:
		break;

	case EPickupHealthType::EPH_HealthSmall:

		PickupParser->SetObjectData(TEXT("Health_Small"));

		PickupParser->Parser();

		SetData();

		break;

	case EPickupHealthType::EPH_HealthMedium:

		PickupParser->SetObjectData(TEXT("Health_Medium"));

		PickupParser->Parser();

		SetData();

		break;

	case EPickupHealthType::EPH_HealthLarge:

		PickupParser->SetObjectData(TEXT("Health_Large"));

		PickupParser->Parser();

		SetData();

		break;

	default:
		break;
	}
}

void APickupBase::SetAmmoData(EPickupAmmoType PickupAmmo)
{
	switch (PickupAmmo)
	{
	case EPickupAmmoType::EPA_None:
		break;

	case EPickupAmmoType::EPH_PistolAmmo:

		PickupParser->SetObjectData(TEXT("PistolAmmo"));

		PickupParser->Parser();

		SetData();

		break;

	case EPickupAmmoType::EPS_RifleAmmo:

		PickupParser->SetObjectData(TEXT("RifleAmmo"));

		PickupParser->Parser();

		SetData();

		break;

	case EPickupAmmoType::EPA_ShotgunAmmo:

		PickupParser->SetObjectData(TEXT("ShotgunAmmo"));

		PickupParser->Parser();

		SetData();

		break;

	default:
		break;
	}
}

void APickupBase::SetArmorData() 
{ 
	PickupParser->SetObjectData("Armor"); 

	PickupParser->Parser();

	SetData();
}

void APickupBase::SetWeaponData(EWeaponName Name)
{
	switch (Name)
	{
	case EWeaponName::EWN_NONE:
		break;

	case EWeaponName::EWN_TT33:

		PickupParser->SetWeaponObjectData("TT33");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_AK47:

		PickupParser->SetWeaponObjectData("AK47");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_AmericanShotgun:

		PickupParser->SetWeaponObjectData("AmericanShotgun");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_BelgianAR:

		PickupParser->SetWeaponObjectData("BelgianAR");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_GermanSMG:

		PickupParser->SetWeaponObjectData("GermanSMG");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_HandCannon:

		PickupParser->SetWeaponObjectData("HandCannon");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_SKS:

		PickupParser->SetWeaponObjectData("SKS");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_XM82:

		PickupParser->SetWeaponObjectData("XM82");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_Bulldog:

		PickupParser->SetWeaponObjectData("Bulldog");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_L86:

		PickupParser->SetWeaponObjectData("L86");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_AK74:

		PickupParser->SetWeaponObjectData("AK74");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_M4A1:
		break;

	case EWeaponName::EWN_NavySMG:

		PickupParser->SetWeaponObjectData("NavySMG");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_ItalianShotgun:

		PickupParser->SetWeaponObjectData("ItalianShotgun");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_SVD:

		PickupParser->SetWeaponObjectData("SVD");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	case EWeaponName::EWN_ShortStrokeAR:

		PickupParser->SetWeaponObjectData("ShortStrokeAR");

		PickupParser->WeaponPickupParser();

		SetWeaponPickupData();

		break;

	default:
		break;
	}
}
