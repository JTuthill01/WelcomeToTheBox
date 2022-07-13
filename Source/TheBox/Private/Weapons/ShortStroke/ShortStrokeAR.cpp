#include "Weapons/ShortStroke/ShortStrokeAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AShortStrokeAR::AShortStrokeAR() = default;

void AShortStrokeAR::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	if (IsValid(WeaponAnimInstance))
	{
		WeaponFireTimer = WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &AShortStrokeAR::ResetCanFire, WeaponFireTimer, false);
	}
}

void AShortStrokeAR::WeaponReload()
{
}

void AShortStrokeAR::ResetIsReloading()
{
}

void AShortStrokeAR::ResetCanFire()
{
	bCanFire = true;
	bCanReload = true;

	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);
}
