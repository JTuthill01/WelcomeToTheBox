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
	virtual void BeginPlay() override;

	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void SetData() override;
};
