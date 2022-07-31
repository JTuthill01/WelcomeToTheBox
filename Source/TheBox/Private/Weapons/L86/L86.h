#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "L86.generated.h"

UCLASS()
class AL86 : public AWeaponBase
{
	GENERATED_BODY()

public:
	AL86();
	
protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

private:
	void ResetCanFireOrReload();
};
