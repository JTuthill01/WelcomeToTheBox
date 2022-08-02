#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "SKS.generated.h"

UCLASS()
class ASKS : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ASKS();

protected:
	virtual void WeaponSetup() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;
};
