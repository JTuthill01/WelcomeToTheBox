#include "Weapons/ShortStroke/ShortStrokeAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"

AShortStrokeAR::AShortStrokeAR() = default;

void AShortStrokeAR::WeaponSetup()
{
	Super::WeaponSetup();

	WeaponParser->SetObjectData("ShortStrokeAR");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.Icon = LoadObject<class UTexture2D>(this, *WeaponFilePaths.IconPath);
	WeapStats.RackSlideSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.RackSlideSoundPath);
	WeapStats.MagOutSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagOutSoundPath);
	WeapStats.MagInSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagInSoundPath);
	WeapStats.FireSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.FireSoundPath);
	WeapStats.AmmoEject = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.AmmoEjectPath);
	WeapStats.FireFX = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.FireFXPath);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}

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

		UGameplayStatics::SpawnSoundAttached(WeapStats.FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &AShortStrokeAR::ResetCanFireOrReload, WeaponFireTimer, false);
	}
}

void AShortStrokeAR::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
	{
		bIsFiring = false;

		WeaponReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

		GetWorldTimerManager().SetTimer(WeaponReloadTimerHandle, this, &AShortStrokeAR::ResetCanFireOrReload, WeaponReloadTimer, false);
	}

	else
		return;
}


void AShortStrokeAR::StopFire()
{
	Super::StopFire();

	bIsFiring = false;
}


void AShortStrokeAR::ResetCanFireOrReload()
{
	bIsReloading = false;
	bIsFiring = false;

	GetWorldTimerManager().ClearAllTimersForObject(this);
}
