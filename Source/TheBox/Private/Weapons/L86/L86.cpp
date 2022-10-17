#include "Weapons/L86/L86.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"
#include <Engine/Texture2D.h>

AL86::AL86() = default;

void AL86::BeginPlay()
{
	Super::BeginPlay();

	SetData();

	/*WeapStats.AmmoEject = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.AmmoEjectPath);
	WeapStats.FireFX = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.FireFXPath);*/
}

void AL86::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(WeapStats.FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());
	}

	else
		return;
}

void AL86::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

void AL86::SetData()
{
	WeaponParser->SetObjectData("L86");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);

	/*WeapStats.RackSlideSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.RackSlideSoundPath);
	WeapStats.MagOutSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagOutSoundPath);
	WeapStats.MagInSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagInSoundPath);
	WeapStats.FireSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.FireSoundPath);*/
}

