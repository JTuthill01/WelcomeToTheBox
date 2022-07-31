#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "BelgianAR.generated.h"

UCLASS()
class ABelgianAR : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABelgianAR();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

	virtual void WeaponSetup() override;

private:
	void ResetCanFireOrReload();
};
