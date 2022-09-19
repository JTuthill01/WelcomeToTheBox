#include "Weapons/XM82/XM82.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

AXM82::AXM82() = default;

void AXM82::BeginPlay()
{
	Super::BeginPlay();

	SetData();
}

void AXM82::WeaponFire()
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

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->AmmoEjectFXMap[EWeaponName::EWN_XM82], EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxerInstance->FireFXMap[EWeaponName::EWN_XM82], FireTransform.GetTranslation(), FireQuat.Rotator());

	}

	else
		return;
}

void AXM82::WeaponReload()
{
	Super::WeaponReload();

	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	else
		return;
}

void AXM82::SetData()
{
	WeaponParser->SetObjectData("XM82");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);

	WeapStats.Icon = LoadObject<class UTexture2D>(this, *WeaponFilePaths.IconPath);
}

