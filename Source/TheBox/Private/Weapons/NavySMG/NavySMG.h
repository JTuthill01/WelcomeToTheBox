#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "NavySMG.generated.h"

UCLASS()
class ANavySMG : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ANavySMG();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;
};
