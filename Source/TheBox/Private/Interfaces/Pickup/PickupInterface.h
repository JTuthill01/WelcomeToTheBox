#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdatePlayerStats(float HealthPickupValue, float ArmorPickupValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateWeaponAmmo(int32 AmmoPickupValue);
};
