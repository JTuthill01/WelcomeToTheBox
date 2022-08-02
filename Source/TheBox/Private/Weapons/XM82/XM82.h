#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "XM82.generated.h"

UCLASS()
class AXM82 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AXM82();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;
};
