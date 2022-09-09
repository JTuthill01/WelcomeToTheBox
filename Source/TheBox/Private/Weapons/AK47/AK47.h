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

protected:
	virtual void BeginPlay() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void SetData() override;
};
