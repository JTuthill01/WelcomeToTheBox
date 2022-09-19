#include "Weapons/Shotguns/Bulldog/Bulldog.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"

ABulldog::ABulldog() = default;

void ABulldog::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	SetData();
}

void ABulldog::WeaponFire()
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
}

void ABulldog::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadStartIndex]);

	float Length = WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadStartIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::ShotgunReloadLoop, Length, false);
}

void ABulldog::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	WeapStats.CurrentMagTotal++;

	WeapStats.CurrentTotalAmmo--;

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadLoopIndex]);

	float Length = WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadLoopIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::UpdateReloadAmmo, Length, false);
}

void ABulldog::ShotgunReloadEnd()
{
	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadEndIndex]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadEndIndex]);
}

void ABulldog::SetData()
{
	WeaponParser->SetObjectData("Bulldog");
	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);

	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
	WeapStats.Type = static_cast<EWeaponType>(InType);
}

void ABulldog::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	OnShotgunReload.Broadcast(WeapStats.CurrentMagTotal, WeapStats.CurrentTotalAmmo);

	if (WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}
