
#pragma once

UENUM(BlueprintType)
enum class EPickupType : uint8 { EPT_None UMETA(DisplayName = "None"), EPT_Weapon UMETA(DisplayName = "Weapon"), EPT_Health UMETA(DisplayName = "Health"), EPT_Ammo UMETA(DisplayName = "Ammo"),
	EPT_Armor UMETA(DisplayName = "Armor") };

UENUM(BlueprintType)
enum class EPickupHealthType : uint8 { EPH_None UMETA(DisplayName = "None"), EPH_HealthSmall UMETA(DisplayName = "Health Small"), EPH_HealthMedium UMETA(DisplayName = "Health Medium"), 
	EPH_HealthLarge UMETA(DisplayName = "Health Large") };

UENUM(BlueprintType)
enum class EPickupAmmoType : uint8 { EPA_None UMETA(DisplayName = "None"), EPH_PistolAmmo UMETA(DisplayName = "Pistol Ammo"), EPS_RifleAmmo UMETA(DisplayName = "Rifle Ammo"), 
	EPA_ShotgunAmmo UMETA(DisplayName = "Shotgun Ammo")};