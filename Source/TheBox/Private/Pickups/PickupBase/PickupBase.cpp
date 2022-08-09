#include "Pickups/PickupBase/PickupBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "JsonComponents/PickupComponent/PickupComponent.h"
#include "Interfaces/Pickup/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Structs/HexColors/Str_CustomHexColors.h"

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

		SetWeaponData(PickupWeaponType);

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

void APickupBase::Setup()
{
	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (IsValid(PlayerRef))
		PlayerRef->Clear.AddDynamic(this, &APickupBase::OnClearViewport);
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

		WeaponPickup(PickupWeaponType);

		OnClearViewport();

		break;

	case EPickupType::EPT_Health:

		HealthPickup(BaseHealthType);

		OnClearViewport();

		break;

	case EPickupType::EPT_Ammo:

		AmmoPickup(PickupAmmoType);

		OnClearViewport();

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
	if (!IsValid(PlayerRef) && !PlayerRef->GetWeaponSlotArray().IsValidIndex(PlayerRef->GetEquippedWeaponIndexUint()))
		return;

	uint8 Temp = PlayerRef->GetEquippedWeaponIndexUint();

	switch (InAmmoType)
	{
	case EPickupAmmoType::EPA_None:
		break;

	case EPickupAmmoType::EPH_PistolAmmo:

		if (PlayerRef->GetWeaponSlotArray()[Temp]->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			if (!PlayerRef->GetWeaponSlotArray()[Temp]->IsAmmoFull())
			{
				PlayerRef->GetWeaponSlotArray()[Temp]->SetTotalAmmo(PickupData.PistolAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPS_RifleAmmo:

		if (PlayerRef->GetWeaponSlotArray()[Temp]->GetWeaponType() == EWeaponType::EWT_Rifle)
		{
			if (!PlayerRef->GetWeaponSlotArray()[Temp]->IsAmmoFull())
			{
				PlayerRef->GetWeaponSlotArray()[Temp]->SetTotalAmmo(PickupData.RifleAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPA_ShotgunAmmo:

		if (PlayerRef->GetWeaponSlotArray()[Temp]->GetWeaponType() == EWeaponType::EWT_Shotgun)
		{
			if (!PlayerRef->GetWeaponSlotArray()[Temp]->IsAmmoFull())
			{
				PlayerRef->GetWeaponSlotArray()[Temp]->SetTotalAmmo(PickupData.ShotgunAmmoValue);

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
	TObjectPtr<UMaterialInstance> NewInstance = LoadObject<UMaterialInstance>(this, *PickupParser->IconFilePathString);

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

void APickupBase::SetDataWeapon()
{
	TObjectPtr<UMaterialInstance> NewInstance = LoadObject<UMaterialInstance>(this, *PickupParser->IconFilePathString);

	PickupData.PickupName = FName(*PickupParser->PickupNameString);
	PickupData.Icon = NewInstance;
	PickupData.PickupType = static_cast<EPickupType>(PickupParser->PType);
	PickupWeaponType = static_cast<EPickupWeaponType>(PickupParser->PWeaponType);
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

void APickupBase::WeaponPickup(EPickupWeaponType InWeaponType) 
{

}

void APickupBase::SetWeaponData(EPickupWeaponType PickupWeapon) 
{
	switch (PickupWeapon)
	{
	case EPickupWeaponType::EPW_NONE:
		break;

	case EPickupWeaponType::EPW_TT33:

		PickupParser->SetObjectData("TT33");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_AK47:

		PickupParser->SetObjectData("AK47");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_AmericanShotgun:

		PickupParser->SetObjectData("AmericanShotgun");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_BelgianAR:

		PickupParser->SetObjectData("BelgianAR");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_GermanSMG:

		PickupParser->SetObjectData("GermanSMG");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_HandCannon:

		PickupParser->SetObjectData("HandCannon");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_SKS:

		PickupParser->SetObjectData("SKS");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_XM82:

		PickupParser->SetObjectData("XM82");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_Bulldog:

		PickupParser->SetObjectData("Bulldog");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_L86:

		PickupParser->SetObjectData("L86");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_AK74:

		PickupParser->SetObjectData("AK74");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_M4A1:
		break;

	case EPickupWeaponType::EPW_NavySMG:

		PickupParser->SetObjectData("NavySMG");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_ItalianShotgun:

		PickupParser->SetObjectData("ItalianShotgun");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_SVD:

		PickupParser->SetObjectData("SVD");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	case EPickupWeaponType::EPW_ShortStrokeAR:

		PickupParser->SetObjectData("ShortStrokeAR");

		PickupParser->WeaponParser();

		SetDataWeapon();

		break;

	default:
		break;
	}
}
