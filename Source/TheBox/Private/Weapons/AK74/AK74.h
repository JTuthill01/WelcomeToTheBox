#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "AK74.generated.h"

UCLASS()
class AAK74 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AAK74();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

private:
	void ResetCanFireOrCanReload();
};
