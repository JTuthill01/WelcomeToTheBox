#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "HandCannon.generated.h"

UCLASS()
class AHandCannon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AHandCannon();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;
};
