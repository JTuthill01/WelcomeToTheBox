#include "Weapons/Shotguns/ItalianShotgun/ItalianShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include <Structs/HexColors/Str_CustomHexColors.h>

AItalianShotgun::AItalianShotgun() = default;

void AItalianShotgun::WeaponSetup()
{
	Super::WeaponSetup();

	WeaponParser->SetObjectData("ItalianShotgun");
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

void AItalianShotgun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AItalianShotgun::WeaponFire()
{
	Super::WeaponFire();

	bIsFiring = true;

	bIsReloading = true;

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

void AItalianShotgun::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	bIsReloading = true;

	bIsFiring = true;

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->ItalianReloadMonatge[ShotgunReloadStartIndex]);

	float Length = WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[ShotgunReloadStartIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AItalianShotgun::ShotgunReloadLoop, Length, false);
}

void AItalianShotgun::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	WeapStats.CurrentMagTotal++;

	WeapStats.CurrentTotalAmmo--;

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->ItalianReloadMonatge[ShotgunReloadLoopIndex]);

	float Length = WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[ShotgunReloadLoopIndex]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AItalianShotgun::UpdateReloadAmmo, Length, false);
}

void AItalianShotgun::ShotgunReloadEnd()
{
	Super::ShotgunReloadEnd();

	WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[ShotgunReloadEndIndex]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->ItalianReloadMonatge[ShotgunReloadEndIndex]);
}

void AItalianShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	OnShotgunReload.Broadcast(WeapStats.CurrentMagTotal, WeapStats.CurrentTotalAmmo);

	if (WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal)
			ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}
