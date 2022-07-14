#include "Weapons/AK47/AK47.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"

AAK47::AAK47() = default;

void AAK47::WeaponSetup()
{
	Super::WeaponSetup();

	uint8 Temp;

	WeaponParser->SetObjectData("AK47");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, Temp);

	WeapStats.Icon = LoadObject<class UTexture2D>(this, *WeaponFilePaths.IconPath);
	WeapStats.RackSlideSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.RackSlideSoundPath);
	WeapStats.MagOutSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagOutSoundPath);
	WeapStats.MagInSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagInSoundPath);
	WeapStats.FireSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.FireSoundPath);
	WeapStats.AmmoEject = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.AmmoEjectPath);
	WeapStats.FireFX = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.FireFXPath);

	WeapStats.FireType = static_cast<EWeaponFireType>(Temp);
}

void AAK47::WeaponFire()
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

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &AAK47::ResetCanFire, WeaponFireTimer, false);
	}
}

void AAK47::WeaponReload()
{
}

void AAK47::StopFire()
{
	bCanFire = true;
	bCanReload = true;
}

void AAK47::ResetCanFire()
{
	bCanFire = true;
	bCanReload = true;

	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);
}
