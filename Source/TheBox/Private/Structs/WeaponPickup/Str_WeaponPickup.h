#pragma once

#include "Enums/PickupEnums/PickupEnums.h"
#include "Str_WeaponPickup.generated.h"

USTRUCT(BlueprintType)
struct FWeaponPickup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMesh> PickupMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IconImage)
	TObjectPtr<class UMaterialInstance> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PickupName)
	FName WeaponPickupName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PickupWidgetText)
	FString PickupWidgetText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Enums)
	EPickupType PickupType;
};