#include "Weapons/Shotguns/AmericanShotgun/AmericanShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"

AAmericanShotgun::AAmericanShotgun() = default;

void AAmericanShotgun::WeaponSetup()
{
	Super::WeaponSetup();

	WeaponParser->SetObjectData("AmericanShotgun");
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

void AAmericanShotgun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AAmericanShotgun::WeaponFire()
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

		GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &AAmericanShotgun::ResetCanFire, WeaponFireTimer, false);
	}
}

void AAmericanShotgun::ShotgunReloadStart()
{
	bIsFiring = false;

	Super::ShotgunReloadStart();

	float Length = PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->AmericanReloadMonatge[ShotgunReloadStartIndex]);

	WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[ShotgunReloadStartIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AAmericanShotgun::ShotgunReloadLoop, Length, false);
}

void AAmericanShotgun::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	WeapStats.CurrentMagTotal++;

	WeapStats.CurrentTotalAmmo--;

	float Length = PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->AmericanReloadMonatge[ShotgunReloadLoopIndex]);

	WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[ShotgunReloadLoopIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AAmericanShotgun::UpdateReloadAmmo, Length, false);
}

void AAmericanShotgun::ShotgunReloadEnd()
{
	WeaponFireTimer = WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[ShotgunReloadEndIndex]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->AmericanReloadMonatge[ShotgunReloadEndIndex]);

	GetWorldTimerManager().SetTimer(WeaponFireTimerHandle, this, &AAmericanShotgun::ResetCanFire, WeaponFireTimer, false);

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);
}

void AAmericanShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	OnShotgunReload.Broadcast(WeapStats.CurrentMagTotal, WeapStats.CurrentTotalAmmo);

	if (WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}

void AAmericanShotgun::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(WeaponFireTimerHandle);

	bIsFiring = false;

	bIsReloading = false;
}
