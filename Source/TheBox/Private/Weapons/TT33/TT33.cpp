#include "Weapons/TT33/TT33.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

ATT33::ATT33() = default;

void ATT33::BeginPlay()
{
	Super::BeginPlay();

	SetData();
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

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->AmmoEjectFXMap[EWeaponName::EWN_TT33], EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->FireFXMap[EWeaponName::EWN_TT33], FireTransform.GetTranslation(), FireQuat.Rotator());

		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponFireMontage);
	}
}

void ATT33::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

void ATT33::SetData()
{
	WeaponParser->SetObjectData("TT33");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}


