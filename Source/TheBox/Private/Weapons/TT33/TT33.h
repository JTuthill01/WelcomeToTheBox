#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "TT33.generated.h"

UCLASS()
class ATT33 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ATT33();

public:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override; 

	virtual void StopFire() override;

private:
	void ResetCanFire();
};
