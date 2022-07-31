#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CurrentWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCurrentWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class ICurrentWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class AWeaponBase* SetCurrentWeaponRefPtr();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCurrentWeaponRefFunc(class AWeaponBase*& InRef);
};
