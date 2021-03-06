#include "Weapons/TT33/TT33.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"

ATT33::ATT33() = default;

void ATT33::WeaponSetup()
{
	Super::WeaponSetup();

	WeaponParser->SetObjectData("TT33");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum);

	WeapStats.Icon = LoadObject<class UTexture2D>(this, *WeaponFilePaths.IconPath);
	WeapStats.RackSlideSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.RackSlideSoundPath);
	WeapStats.MagOutSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagOutSoundPath);
	WeapStats.MagInSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagInSoundPath);
	WeapStats.FireSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.FireSoundPath);
	WeapStats.AmmoEject = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.AmmoEjectPath);
	WeapStats.FireFX = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.FireFXPath);
}

void ATT33::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	if (IsValid(WeaponMesh->GetAnimInstance()))
	{
		UGameplayStatics::SpawnSoundAttached(WeapStats.FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		WeaponFireTimer = WeaponMesh->GetAnimInstance()->Montage_Play(WeaponFireMontage);

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &ATT33::ResetCanFireOrReload, WeaponFireTimer, false);
	}
}

void ATT33::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
	{
		WeaponReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

		GetWorldTimerManager().SetTimer(WeaponReloadTimerHandle, this, &ATT33::ResetCanFireOrReload, WeaponReloadTimer, false);
	}

	else
		return;
}

void ATT33::StopFire()
{
	Super::StopFire();

	bCanFire = true;
	bCanReload = true;
	bIsFiring = false;
}

void ATT33::ResetCanFireOrReload()
{
	bCanFire = true;
	bCanReload = true;
	bIsFiring = false;

	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);
	GetWorldTimerManager().ClearTimer(WeaponReloadTimerHandle);
}
