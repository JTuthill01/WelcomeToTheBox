#include "Weapons/Shotguns/Bulldog/Bulldog.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"

ABulldog::ABulldog() = default;

void ABulldog::WeaponSetup()
{
	Super::WeaponSetup();

	WeaponParser->SetObjectData("Bulldog");
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

void ABulldog::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
		WeaponFireTimer = WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(WeapStats.FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeapStats.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &ABulldog::ResetCanFire, WeaponFireTimer, false);
	}
}

void ABulldog::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	bIsFiring = false;

	float Length = PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadStartIndex]);

	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadStartIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::ShotgunReloadLoop, Length, false);
}

void ABulldog::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	WeapStats.CurrentMagTotal++;

	WeapStats.CurrentTotalAmmo--;

	float Length = PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadLoopIndex]);

	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadLoopIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::UpdateReloadAmmo, Length, false);
}

void ABulldog::ShotgunReloadEnd()
{
	WeaponFireTimer = WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[ShotgunReloadEndIndex]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->BulldogReloadMonatge[ShotgunReloadEndIndex]);

	GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &ABulldog::ResetCanFire, WeaponFireTimer, false);

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);
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

void ABulldog::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);

	bIsFiring = false;

	bIsReloading = false;
}
