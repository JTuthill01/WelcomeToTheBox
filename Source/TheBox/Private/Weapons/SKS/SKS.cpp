#include "Weapons/SKS/SKS.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

ASKS::ASKS() = default;

void ASKS::BeginPlay()
{
	Super::BeginPlay();

	SetData();
}

void ASKS::SetData()
{
	WeaponParser->SetObjectData("SKS");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}

void ASKS::WeaponFire()
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

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->AmmoEjectFXMap[EWeaponName::EWN_SKS], EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->FireFXMap[EWeaponName::EWN_SKS], FireTransform.GetTranslation(), FireQuat.Rotator());
	}

	else
		return;
}

void ASKS::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

