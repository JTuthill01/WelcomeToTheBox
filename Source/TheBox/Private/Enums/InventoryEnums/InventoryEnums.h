#pragma once

UENUM(BlueprintType)
enum class EInventoryType : uint8 { EIT_None UMETA(DisplayName = "None"), EIT_Weapon UMETA(DisplayName = "Weapon"), EIT_Health UMETA(DisplayName = "Health"), EIT_HellsElixir UMETA(DisplayName = "Hells Elixir") };