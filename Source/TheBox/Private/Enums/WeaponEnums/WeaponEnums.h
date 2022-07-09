#pragma once

UENUM(BlueprintType)
enum class EHasWeapon : uint8 { EHW_NoWeapon UMETA(DisplayName = "No Weapon"), EHW_HasWeapon UMETA(DisplayName = "Has Weapon") };

UENUM(BlueprintType)
enum class EWeaponSlot : uint8 { EWS_First_Slot UMETA(DisplayName = "First Slot"), EWS_Second_Slot UMETA(DisplayName = "Second Slot"), EWS_Third_Slot UMETA(DisplayName = "Third Slot") };

UENUM(BlueprintType)
enum class EWeaponName : uint8 { EWN_NONE UMETA(DisplayName = "None"), EWN_TT33 UMETA(DisplayName = "TT33"), EWN_AK47 UMETA(DisplayName = "AK47"), EWN_AmericanShotgun UMETA(DisplayName = "American Shotgun"), EWN_BelgianAR UMETA(DisplayName = "Belgian AR"),
	EWN_GermanSMG UMETA(DisplayName = "German SMG"), EWN_HandCannon UMETA(DisplayName = "Hand Cannon"), EWN_SKS UMETA(DisplayName = "SKS"), EWN_XM82 UMETA(DisplayName = "XM82"), EWN_Bulldog UMETA(DisplayName = "Bulldog"),
	EWN_L86 UMETA(DisplayName = "L86"), EWN_AK74 UMETA(DisplayName = "AK74"), EWN_M4A1 UMETA(DisplayName = "M4A1"), EWN_NavySMG UMETA(DisplayName = "Navy SMG"), EWN_ItalianShotgun UMETA(DisplayName = "Italian"),
	EWN_SVD UMETA(DisplayName = "SVD"), EWN_ShortStrokeAR UMETA(DisplayName = "ShortStrokeAR") };

UENUM(BlueprintType)
enum class EWeaponFireType : uint8 { EWFT_None UMETA(DisplayName = "None"), EWFT_Hitscan UMETA(DisplayName = "Hitscan"), EWFT_Projectile UMETA(DisplayName = "Projectile"), 
	EWFT_SpreadScan UMETA(DisplayName = "Spread Scan") };