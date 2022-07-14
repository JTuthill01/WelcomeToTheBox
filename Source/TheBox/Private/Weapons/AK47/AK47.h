#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "AK47.generated.h"

UCLASS()
class AAK47 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AAK47();

public:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

protected:
	virtual void WeaponSetup() override;

private:
	void ResetCanFire();
};
