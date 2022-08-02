#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/WeaponPickup/Str_WeaponPickup.h"
#include "PickupComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupComponent();

	FORCEINLINE void SetObjectData(FString InObjectString) { ObjectString = InObjectString; }
	FORCEINLINE void SetWeaponObjectData(FString InObjectString) { WeaponObjectString = InObjectString; }

public:
	UFUNCTION()
	void Parser();

	UFUNCTION()
	void WeaponPickupParser();

protected:

	virtual void BeginPlay() override;

private:
	void InitializerList();

public:

#pragma region Strings
	
	FString MeshFilePathString;

	FString IconFilePathString;

	FString PickupNameString;

	FString NameString;

	FString WidgetTextString;

#pragma endregion

#pragma region uints

	uint8 PType;

	uint8 PHealthType;

	uint8 PAmmoType;

#pragma endregion

#pragma region floats

	float HealthValue;

	float ArmValue;

#pragma endregion

#pragma region int32s

	int32 PAmmoValue;

	int32 RAmmoValue;

	int32 SAmmoValue;

#pragma endregion

private:
	FString ObjectString;

	FString WeaponObjectString;
};
