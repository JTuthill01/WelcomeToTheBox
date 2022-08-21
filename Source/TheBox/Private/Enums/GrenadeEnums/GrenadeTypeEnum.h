#pragma once

UENUM(BlueprintType)
enum class EGrenadeType : uint8 { EGT_None UMETA(DisplayName = "None"), EGT_Frag UMETA(DisplayName = "Frag"), EGT_Incendary UMETA(DisplayName = "Incendary"), EGT_Electric UMETA(DisplayName = "Electric"),
	EGT_FlashBang UMETA(DisplayName = "Flash Bang"), EGT_Corrosive UMETA(DisplayName = "Corrosive"), EGT_Smoke UMETA(DisplayName = "Smoke") };