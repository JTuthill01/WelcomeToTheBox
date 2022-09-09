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

protected:

	virtual void BeginPlay() override;

	virtual void SetData() override;
};
