
#pragma once

UENUM(BlueprintType)
enum class EPickupType : uint8 { EPT_None UMETA(DisplayName = "None"), EPT_Weapon UMETA(DisplayName = "Weapon"), EPT_Health UMETA(DisplayName = "Health"), EPT_Ammo UMETA(DisplayName = "Ammo"),
	EPT_Armor UMETA(DisplayName = "Armor"), EPT_Grenade UMETA(DisplayName = "Grenade") };

UENUM(BlueprintType)
enum class EPickupHealthType : uint8 { EPH_None UMETA(DisplayName = "None"), EPH_HealthSmall UMETA(DisplayName = "Health Small"), EPH_HealthMedium UMETA(DisplayName = "Health Medium"), 
	EPH_HealthLarge UMETA(DisplayName = "Health Large") };

UENUM(BlueprintType)
enum class EPickupAmmoType : uint8 { EPA_None UMETA(DisplayName = "None"), EPH_PistolAmmo UMETA(DisplayName = "Pistol Ammo"), EPS_RifleAmmo UMETA(DisplayName = "Rifle Ammo"), 
	EPA_ShotgunAmmo UMETA(DisplayName = "Shotgun Ammo")};

UENUM(BlueprintType)
enum class EPickupGrenadeType : uint8 { EPGT_None UMETA(DisplayName = "None"), EPGT_Frag UMETA(DisplayName = "Frag"), EPGT_Incendary UMETA(DisplayName = "Incendary"), EPGT_Electric UMETA(DisplayName = "Electric"),
	EPGT_FlashBang UMETA(DisplayName = "Flash Bang"), EPGT_Corrosive UMETA(DisplayName = "Corrosive"), EPGT_Smoke UMETA(DisplayName = "Smoke") };

UENUM(BlueprintType)
enum class EPickupWeaponType : uint8 { EPW_NONE UMETA(DisplayName = "None"), EPW_TT33 UMETA(DisplayName = "TT33"), EPW_AK47 UMETA(DisplayName = "AK47"), EPW_AmericanShotgun UMETA(DisplayName = "American Shotgun"), 
	EPW_BelgianAR UMETA(DisplayName = "Belgian AR"), EPW_GermanSMG UMETA(DisplayName = "German SMG"), EPW_HandCannon UMETA(DisplayName = "Hand Cannon"), EPW_SKS UMETA(DisplayName = "SKS"), 
	EPW_XM82 UMETA(DisplayName = "XM82"), EPW_Bulldog UMETA(DisplayName = "Bulldog"), EPW_L86 UMETA(DisplayName = "L86"), EPW_AK74 UMETA(DisplayName = "AK74"), EPW_M4A1 UMETA(DisplayName = "M4A1"), 
	EPW_NavySMG UMETA(DisplayName = "Navy SMG"), EPW_ItalianShotgun UMETA(DisplayName = "Italian Shotgun"), EPW_SVD UMETA(DisplayName = "SVD"), EPW_ShortStrokeAR UMETA(DisplayName = "ShortStrokeAR") };