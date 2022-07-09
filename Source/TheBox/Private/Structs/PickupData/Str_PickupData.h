#pragma once

#include "Engine/DataTable.h"
#include "Enums/PickupEnums/PickupEnums.h"
#include "Str_PickupData.generated.h"

USTRUCT(BlueprintType)
struct FPickupData : public FTableRowBase
{
    
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName PickupName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMaterialInstance* Icon;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Enums)
	EPickupType PickupType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Enums)
	EPickupHealthType PickupHealthType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Enums)
	EPickupAmmoType PickupAmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	float HealthPackValue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Armor)
	float ArmorValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
	int32 PistolAmmoValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
	int32 RifleAmmoValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
	int32 ShotgunAmmoValue;
};