#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Bulldog.generated.h"

UCLASS()
class ABulldog : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABulldog();

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
		TArray<TObjectPtr<class UAnimMontage>> BulldogReloadMonatge;
};
