#include "Weapons/TT33/TT33.h"
#include "NiagaraFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

ATT33::ATT33() = default;

void ATT33::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	if (WeaponAnimInstance)
	{
		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		WeaponFireTimer = WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &ATT33::ResetCanFire, WeaponFireTimer, false);
	}
}

void ATT33::WeaponReload()
{
}

void ATT33::StopFire()
{
}

void ATT33::ResetCanFire()
{
	bCanFire = true;
	bCanReload = true;

	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);
}
