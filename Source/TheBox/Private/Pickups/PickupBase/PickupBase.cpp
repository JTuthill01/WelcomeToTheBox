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
APickupBase::APickupBase() : HealthValue(0.F), ArmorValue(0.F), MaxWeapons(4)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Pickup Root"));
	SetRootComponent(PickupRoot);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(PickupRoot);
	BaseMesh->SetCastShadow(false);
	BaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	PickupParser = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup Parser"));
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	Setup();

	LoadBP();
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

	case EPickupType::EPT_Grenade:

		SetGrenadeData(PickupGrenadeType);

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

		WeaponPickup(WeaponName);

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

	case EPickupType::EPT_Grenade:

		GrenadePickup();

		break;

	default:
		break;
	}
}

void APickupBase::OnClearViewport()
{
	
}

void APickupBase::WeaponPickup(EWeaponName InWeaponName)
{
	bool bIsSuccessful;

	if (!PlayerRef->GetWeaponMap().Find(InWeaponName) && PlayerRef->GetWeaponMap().Num() <= MaxWeapons)
	{
		PlayerRef->SpawnWeaponMap(LoadedBpAsset, bIsSuccessful);

		if (bIsSuccessful)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSFX, GetActorLocation(), GetActorRotation());

			Destroy();
		}
	}

	else
		return;
}

void APickupBase::GrenadePickup()
{
	if (IsValid(PlayerRef))
	{
		if (!PlayerRef->HasMaxGrenades())
		{
			PlayerRef->SetGrenadeCount(PickupData.GrenadePickupValue);

			Destroy();
		}

		else
			return;
	}

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
	if (!IsValid(PlayerRef))
		return;

	switch (InAmmoType)
	{
	case EPickupAmmoType::EPA_None:
		break;

	case EPickupAmmoType::EPH_PistolAmmo:

		if (PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			if (!PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->IsAmmoFull())
			{
				PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->SetTotalAmmo(PickupData.PistolAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPS_RifleAmmo:

		if (PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->GetWeaponType() == EWeaponType::EWT_Rifle)
		{
			if (!PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->IsAmmoFull())
			{
				PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->SetTotalAmmo(PickupData.RifleAmmoValue);

				Destroy();
			}
		}

		break;

	case EPickupAmmoType::EPA_ShotgunAmmo:

		if (PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->GetWeaponType() == EWeaponType::EWT_Shotgun)
		{
			if (!PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->IsAmmoFull())
			{
				PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->SetTotalAmmo(PickupData.ShotgunAmmoValue);

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
	TObjectPtr<UStaticMesh> NewMesh = LoadObject<UStaticMesh>(this, *PickupParser->MeshFilePathString);

	BaseMesh->SetStaticMesh(NewMesh);

	PickupData.PickupName = FName(*PickupParser->PickupNameString);
	PickupData.Icon = NewInstance;
	PickupData.PickupWidgetText = FText::FromString(PickupParser->WidgetTextString);
	PickupData.PickupType = static_cast<EPickupType>(PickupParser->PType);
	PickupData.PickupHealthType = static_cast<EPickupHealthType>(PickupParser->PHealthType);
	PickupData.PickupAmmoType = static_cast<EPickupAmmoType>(PickupParser->PAmmoType);
	PickupData.PickupGrenadeType = static_cast<EPickupGrenadeType>(PickupParser->PGrenadeType);
	PickupData.HealthPackValue = PickupParser->HealthValue;
	PickupData.ArmorValue = PickupParser->ArmValue;
	PickupData.PistolAmmoValue = PickupParser->PAmmoValue;
	PickupData.RifleAmmoValue = PickupParser->RAmmoValue;
	PickupData.ShotgunAmmoValue = PickupParser->SAmmoValue;
	PickupData.GrenadePickupValue = PickupParser->GrenadeValue;
}

void APickupBase::SetDataWeapon()
{
	TObjectPtr<UMaterialInstance> NewInstance = LoadObject<UMaterialInstance>(this, *PickupParser->IconFilePathString);
	TObjectPtr<UStaticMesh> NewMesh = LoadObject<UStaticMesh>(this, *PickupParser->MeshFilePathString);

	PickupSFX = LoadObject<USoundBase>(this, *PickupParser->PickupSoundFilePath);

	BaseMesh->SetStaticMesh(NewMesh);

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

		WeaponName = EWeaponName::EWN_TT33;

		break;

	case EPickupWeaponType::EPW_AK47:

		PickupParser->SetObjectData("AK47");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_AK47;

		break;

	case EPickupWeaponType::EPW_AmericanShotgun:

		PickupParser->SetObjectData("AmericanShotgun");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_AmericanShotgun;

		break;

	case EPickupWeaponType::EPW_BelgianAR:

		PickupParser->SetObjectData("BelgianAR");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_BelgianAR;

		break;

	case EPickupWeaponType::EPW_GermanSMG:

		PickupParser->SetObjectData("GermanSMG");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_GermanSMG;

		break;

	case EPickupWeaponType::EPW_HandCannon:

		PickupParser->SetObjectData("HandCannon");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_HandCannon;

		break;

	case EPickupWeaponType::EPW_SKS:

		PickupParser->SetObjectData("SKS");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_SKS;

		break;

	case EPickupWeaponType::EPW_XM82:

		PickupParser->SetObjectData("XM82");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_XM82;

		break;

	case EPickupWeaponType::EPW_Bulldog:

		PickupParser->SetObjectData("Bulldog");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_Bulldog;

		break;

	case EPickupWeaponType::EPW_L86:

		PickupParser->SetObjectData("L86");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_L86;

		break;

	case EPickupWeaponType::EPW_AK74:

		PickupParser->SetObjectData("AK74");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_AK74;

		break;

	case EPickupWeaponType::EPW_M4A1:
		break;

	case EPickupWeaponType::EPW_NavySMG:

		PickupParser->SetObjectData("NavySMG");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_NavySMG;

		break;

	case EPickupWeaponType::EPW_ItalianShotgun:

		PickupParser->SetObjectData("ItalianShotgun");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_ItalianShotgun;

		break;

	case EPickupWeaponType::EPW_SVD:

		PickupParser->SetObjectData("SVD");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_SVD;

		break;

	case EPickupWeaponType::EPW_ShortStrokeAR:

		PickupParser->SetObjectData("ShortStrokeAR");

		PickupParser->WeaponParser();

		SetDataWeapon();

		WeaponName = EWeaponName::EWN_ShortStrokeAR;

		break;

	default:
		break;
	}
}

void APickupBase::SetGrenadeData(EPickupGrenadeType Grenade)
{
	switch (Grenade)
	{
	case EPickupGrenadeType::EPGT_None:
		break;

	case EPickupGrenadeType::EPGT_Frag:

		PickupParser->SetObjectData("Frag_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	case EPickupGrenadeType::EPGT_Incendary:

		PickupParser->SetObjectData("Incendary_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	case EPickupGrenadeType::EPGT_Electric:

		PickupParser->SetObjectData("Electric_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	case EPickupGrenadeType::EPGT_FlashBang:

		PickupParser->SetObjectData("FlashBang_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	case EPickupGrenadeType::EPGT_Corrosive:

		PickupParser->SetObjectData("Corrosive_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	case EPickupGrenadeType::EPGT_Smoke:

		PickupParser->SetObjectData("Smoke_Grenade");

		PickupParser->Parser();

		SetData();

		break;

	default:
		break;
	}
}

void APickupBase::LoadBP()
{
	switch (PickupWeaponType)
	{
	case EPickupWeaponType::EPW_NONE:
		break;

	case EPickupWeaponType::EPW_TT33:

		PickupParser->LoadParser("TT33", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_AK47:

		PickupParser->LoadParser("AK47", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_AmericanShotgun:

		PickupParser->LoadParser("AmericanShotgun", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_BelgianAR:

		PickupParser->LoadParser("BelgianAR", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_GermanSMG:

		PickupParser->LoadParser("GermanSMG", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_HandCannon:

		PickupParser->LoadParser("HandCannon", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_SKS:

		PickupParser->LoadParser("SKS", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_XM82:

		PickupParser->LoadParser("XM82", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_Bulldog:

		PickupParser->LoadParser("Bulldog", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_L86:

		PickupParser->LoadParser("L86", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_AK74:

		PickupParser->LoadParser("AK74", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_M4A1:
		break;

	case EPickupWeaponType::EPW_NavySMG:

		PickupParser->LoadParser("NavySMG", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_ItalianShotgun:

		PickupParser->LoadParser("ItalianShotgun", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_SVD:

		PickupParser->LoadParser("SVD", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	case EPickupWeaponType::EPW_ShortStrokeAR:

		PickupParser->LoadParser("ShortStrokeAR", WeaponBPFilePath);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(WeaponBPFilePath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();

		break;

	default:
		break;
	}
}

