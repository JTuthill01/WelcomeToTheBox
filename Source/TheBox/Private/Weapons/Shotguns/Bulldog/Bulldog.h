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

	virtual void ShotgunReloadStart() override;

	virtual void ShotgunReloadLoop() override;

	virtual void ShotgunReloadEnd() override;

	virtual void SetData() override;

private:
	void UpdateReloadAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<class UAnimMontage>> BulldogReloadMonatge;
};
