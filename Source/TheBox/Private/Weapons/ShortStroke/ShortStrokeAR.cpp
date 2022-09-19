#include "Weapons/ShortStroke/ShortStrokeAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

AShortStrokeAR::AShortStrokeAR() = default;

void AShortStrokeAR::BeginPlay()
{
	Super::BeginPlay();

	SetData();
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
		WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(WeapStats.FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->AmmoEjectFXMap[EWeaponName::EWN_ShortStrokeAR], EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->FireFXMap[EWeaponName::EWN_ShortStrokeAR], FireTransform.GetTranslation(), FireQuat.Rotator());
	}
}

void AShortStrokeAR::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

void AShortStrokeAR::SetData()
{
	WeaponParser->SetObjectData("ShortStrokeAR");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}


