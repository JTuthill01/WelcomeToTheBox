#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "ShortStrokeAR.generated.h"

UCLASS()
class AShortStrokeAR : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AShortStrokeAR();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
	void ResetCanFire();
};
