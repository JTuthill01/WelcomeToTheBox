#include "Weapons/BelgianAR/BelgianAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

ABelgianAR::ABelgianAR() = default;

void ABelgianAR::BeginPlay()
{
	Super::BeginPlay();

	SetData();
}

void ABelgianAR::WeaponFire()
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

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->AmmoEjectFXMap[EWeaponName::EWN_BelgianAR], EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->FireFXMap[EWeaponName::EWN_BelgianAR], FireTransform.GetTranslation(), FireQuat.Rotator());
	}

	else
		return;
}

void ABelgianAR::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

void ABelgianAR::SetData()
{
	WeaponParser->SetObjectData("BelgianAR");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}

