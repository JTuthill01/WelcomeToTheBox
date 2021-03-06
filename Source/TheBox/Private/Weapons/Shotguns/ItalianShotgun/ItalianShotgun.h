#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "ItalianShotgun.generated.h"

UCLASS()
class AItalianShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AItalianShotgun();

protected:
	virtual void BeginPlay() override;

	virtual void WeaponFire() override;

	virtual void WeaponSetup() override;

	virtual void ShotgunReloadStart() override;

	virtual void ShotgunReloadLoop() override;

	virtual void ShotgunReloadEnd() override;

private:
	void UpdateReloadAmmo();
	void ResetCanFire();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> ItalianReloadMonatge;
};
