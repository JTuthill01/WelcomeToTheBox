#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "SVD.generated.h"

UCLASS()
class ASVD : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ASVD();

public:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

	virtual void StopFire() override;

protected:
	virtual void WeaponSetup() override;

private:
	UFUNCTION()
	void ResetCanFireOrCanReload();
};
