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

	virtual void WeaponSetup() override;

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;
};
