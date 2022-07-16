#pragma once

#include "Str_WeaponStringData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStringData
{
    GENERATED_BODY()

    FString IconPath;

    FString RackSlideSoundPath;

	FString MagOutSoundPath;

	FString MagInSoundPath;

	FString FireSoundPath;

	FString AmmoEjectPath;

	FString FireFXPath;

	FString StringToInt;
};