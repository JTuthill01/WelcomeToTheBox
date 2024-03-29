#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "ImpactPhysicalMaterial/ImpactPhysicalMaterial.h"
#include "JsonComponents/WeaponComponent/WeaponComponentParser.h"
#include "Structs/HexColors/Str_CustomHexColors.h"
#include "Projectiles/ProjectileBase.h"
#include "Widgets/Reload/ReloadWidget.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

// Sets default values
AWeaponBase::AWeaponBase() : SocketName(NAME_None), ShotgunPellets(6), Range(4'500), SpreadAngle(8.89F), bCanShotgunFireOrReload(true), EjectQuat(FQuat(0.F)),
	FireQuat(FQuat(0.F)), InUintToEnum(0), ShotgunReloadStartIndex(0), ShotgunReloadLoopIndex(1), ShotgunReloadEndIndex(2)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Root"));
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCastShadow(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->bReturnMaterialOnMove = true;
	WeaponMesh->SetupAttachment(WeaponRoot);

	WeaponParser = CreateDefaultSubobject<UWeaponComponentParser>(TEXT("Weapon Parser"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeVariables();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::InitializeVariables()
{
	if (IsValid(WeaponMesh))
		WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	BoxerInstance = Cast<UTheBoxGameInstance>(GetGameInstance());
}

//void AWeaponBase::SetData(FString NewWeaponName)
//{
//	WeaponParser->SetObjectData(NewWeaponName);
//	WeaponParser->WeaponParser(WeapStats, WeaponFilePaths, InUintToEnum, InType);
//
//	WeapStats.Icon = LoadObject<class UTexture2D>(this, *WeaponFilePaths.IconPath);
//	WeapStats.RackSlideSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.RackSlideSoundPath);
//	WeapStats.MagOutSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagOutSoundPath);
//	WeapStats.MagInSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.MagInSoundPath);
//	WeapStats.FireSound = LoadObject<class USoundBase>(this, *WeaponFilePaths.FireSoundPath);
//	WeapStats.AmmoEject = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.AmmoEjectPath);
//	WeapStats.FireFX = LoadObject<class UNiagaraSystem>(this, *WeaponFilePaths.FireFXPath);
//
//	WeapStats.FireType = static_cast<EWeaponFireType>(InUintToEnum);
//	WeapStats.Type = static_cast<EWeaponType>(InType);
//}

void AWeaponBase::BulletTrace()
{
	if (!IsValid(PlayerRef))
		return;

	FHitResult HitResult;

	FVector Start{ PlayerRef->GetPlayerCamera()->GetComponentLocation() };
	FVector Rotation{ PlayerRef->GetPlayerCamera()->GetComponentRotation().Vector() };
	FVector End{ Start + Rotation * 10'000.F };
	FVector Scale = FVector(1.F);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerRef);
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bHasBeenHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bHasBeenHit)
	{
		CreateImpactFX(HitResult);

		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), WeapStats.DamageAmount, HitResult.ImpactPoint, HitResult, nullptr, this, UDamageType::StaticClass());
	}
}

void AWeaponBase::CreateImpactFX(FHitResult HitResult)
{
	if (TObjectPtr<UImpactPhysicalMaterial> PhysMat = Cast<UImpactPhysicalMaterial>(HitResult.PhysMaterial))
	{
		if (TObjectPtr<USoundBase> ImpactSound = PhysMat->LineTraceImpactEffect.ImpactSound)
		{
			FVector Location = HitResult.Location;

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
		}

		if (TObjectPtr<UNiagaraSystem> HitFX = Cast<UNiagaraSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX, HitResult.Location, Rotation);
		}

		else if (TObjectPtr<UParticleSystem> ParticleFX = Cast<UParticleSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			FVector Location = HitResult.Location;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX, Location, Rotation);
		}

		if (TObjectPtr<UMaterialInstance> ImpactDecal = PhysMat->LineTraceImpactEffect.ImpactDecal)
		{
			if (TObjectPtr<USceneComponent> HitComponent = HitResult.GetComponent())
			{
				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);

				Rotation.Pitch += 180.0f;

				FVector DecalSize = PhysMat->LineTraceImpactEffect.DecalSize;

				float DecalLifetime = PhysMat->LineTraceImpactEffect.DecalLifeTime;

				UGameplayStatics::SpawnDecalAttached(ImpactDecal, DecalSize, HitComponent, NAME_None,
					HitResult.Location, Rotation, EAttachLocation::KeepWorldPosition, DecalLifetime);
			}
		}
	}
}

void AWeaponBase::WeaponFire()
{
	if (!IsValid(PlayerRef))
		return;

	WeapStats.CurrentMagTotal--;

	OnWeaponFire.Broadcast(WeapStats.CurrentMagTotal);

	if (WeapStats.CurrentMagTotal <= 0)
		WeapStats.CurrentMagTotal = 0;

	SpawnReloadWidget();

	switch (WeapStats.FireType)
	{
	case EWeaponFireType::EWFT_None:
		break;

	case EWeaponFireType::EWFT_Hitscan:

		BulletTrace();

		break;

	case EWeaponFireType::EWFT_Projectile:

		SpawnProjectile();

		break;

	case EWeaponFireType::EWFT_SpreadScan:

		ShotgunFireMulti(ShotgunPellets);

		break;

	default:
		break;
	}
}

void AWeaponBase::WeaponReload()
{
	HideReloadWidget();

	if (!IsValid(PlayerRef))
		return;

	if (HasAmmoForReload() && !IsMagFull())
	{
		PlayerRef->PlayerReloadWeapon();

		WeapStats.CurrentTotalAmmo -= WeapStats.MaxMagTotal - WeapStats.CurrentMagTotal;

		WeapStats.CurrentMagTotal = UKismetMathLibrary::Min(WeapStats.MaxMagTotal, WeapStats.CurrentTotalAmmo);

		WeapStats.CurrentMagTotal = FMath::Clamp(WeapStats.CurrentMagTotal, 0, WeapStats.MaxMagTotal);

		WeapStats.CurrentTotalAmmo = FMath::Clamp(WeapStats.CurrentTotalAmmo, 0, WeapStats.MaxTotalAmmo);

		OnWeaponReload.Broadcast(WeapStats.CurrentMagTotal, WeapStats.CurrentTotalAmmo);
	}
}

void AWeaponBase::ShotgunFireMulti(int32 InShotgunPelletCount)
{
	if (!IsValid(PlayerRef))
		return;

	for (int32 i = 0; i < InShotgunPelletCount; ++i)
	{
		FVector Start{ WeaponMesh->GetSocketLocation("Fire_FX_Slot") };
		FVector Middle{ (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(PlayerRef->GetPlayerCamera()->GetForwardVector(), SpreadAngle)) * Range };
		FVector End{ Start + Middle };

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(PlayerRef);
		ActorsToIgnore.Add(this);

		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

		TArray<FHitResult> OutResult;

		bHasHitOccured = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, OutResult, true);

		if (bHasHitOccured)
		{
			for (auto&& Result : OutResult)
			{
				CreateImpactFX(Result);

				UGameplayStatics::ApplyPointDamage(Result.GetActor(), WeapStats.DamageAmount, Result.ImpactPoint, Result, nullptr, this, UDamageType::StaticClass());
			}
		}
	}
}

void AWeaponBase::SpawnProjectile()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;
	FVector Scale{ FVector::OneVector };
	FVector SocketLocation{ WeaponMesh->GetSocketLocation("Fire_FX_Slot") };
	FRotator Rotation{ PlayerRef->GetControlRotation() };

	SpawnTransform = UKismetMathLibrary::MakeTransform(SocketLocation, Rotation, Scale);

	Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileToSpawn, SpawnTransform, SpawnInfo);
}

void AWeaponBase::SetTotalAmmo(int32 NewAmmoValue)
{
	WeapStats.CurrentTotalAmmo += NewAmmoValue;

	if (WeapStats.CurrentTotalAmmo > WeapStats.MaxTotalAmmo)
		WeapStats.CurrentTotalAmmo = WeapStats.MaxTotalAmmo;

	OnNewTotalAmmo.Broadcast(WeapStats.CurrentTotalAmmo);
}

void AWeaponBase::SpawnReloadWidget()
{
	if (WeapStats.CurrentMagTotal <= WeapStats.LowAmmo)
	{
		if (!IsValid(ReloadWidget))
		{
			ReloadWidget = CreateWidget<UReloadWidget>(GetWorld(), ReloadWidgetSub);

			if (IsValid(ReloadWidget))
				ReloadWidget->AddToViewport(-1);

			else
				return;
		}

		else if (!ReloadWidget->IsInViewport())
			ReloadWidget->AddToViewport(-1);

		else if (ReloadWidget->IsInViewport() && ReloadWidget->GetVisibility() == ESlateVisibility::Collapsed)
			ReloadWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AWeaponBase::HideReloadWidget()
{
	if (IsValid(ReloadWidget))
		ReloadWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AWeaponBase::ShotgunReloadStart() {}

void AWeaponBase::SetData()
{
}

void AWeaponBase::ShotgunReloadLoop() {}

void AWeaponBase::ShotgunReloadEnd() {}

bool AWeaponBase::MagHasAmmo() { return WeapStats.CurrentMagTotal > 0; }

bool AWeaponBase::HasAmmoForReload() { return WeapStats.CurrentTotalAmmo > 0; }

bool AWeaponBase::IsMagFull() { return WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal; }

bool AWeaponBase::CanFireOrReload() { return !WeaponAnimInstance->Montage_IsPlaying(WeaponFireMontage) && !WeaponAnimInstance->Montage_IsPlaying(WeaponReloadMontage); }

bool AWeaponBase::IsAmmoFull() { return WeapStats.CurrentTotalAmmo >= WeapStats.MaxTotalAmmo; }

bool AWeaponBase::CanShotgunFireOrReload() { return !WeaponAnimInstance->Montage_IsPlaying(WeaponFireMontage) && bCanShotgunFireOrReload && HasAmmoForReload() && !IsMagFull(); }

bool AWeaponBase::IsWeaponFiringOrReloading() { return WeaponAnimInstance->Montage_IsPlaying(WeaponFireMontage) || WeaponAnimInstance->Montage_IsPlaying(WeaponReloadMontage); }

void AWeaponBase::StopFire() {}

