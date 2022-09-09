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
	virtual void BeginPlay() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void SetData() override;
};
