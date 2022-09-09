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

	virtual void ShotgunReloadStart() override;

	virtual void ShotgunReloadLoop() override;

	virtual void ShotgunReloadEnd() override;

	virtual void SetData() override;

private:
	void UpdateReloadAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> ItalianReloadMonatge;
};
