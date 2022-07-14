#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "ItalianShotgun.generated.h"

UCLASS()
class AItalianShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AItalianShotgun();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void WeaponSetup() override;
};
