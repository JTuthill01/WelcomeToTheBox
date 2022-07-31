#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "GermanSMG.generated.h"

UCLASS()
class AGermanSMG : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGermanSMG();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

private:
	void ResetCanFireOrReload();
};
