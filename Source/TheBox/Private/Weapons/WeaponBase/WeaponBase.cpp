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
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponBase::AWeaponBase() : SocketName(NAME_None), ShotgunPellets(6), Range(4'500), SpreadAngle(8.89F), bIsReloading(false), bIsFiring(false), EjectQuat(FQuat(0.F)),
	FireQuat(FQuat(0.F)), WeaponFireTimer(0.F), WeaponReloadTimer(0.F), InUintToEnum(0), ShotgunReloadStartIndex(0), ShotgunReloadLoopIndex(1), ShotgunReloadEndIndex(2)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	ResetFireOrReload(DeltaTime);
}

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponSetup();
}

void AWeaponBase::InitializeVariables()
{
	if (IsValid(WeaponMesh))
		WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

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
		CreateImpactFX(HitResult);
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

	switch (WeapStats.FireType)
	{
	case EWeaponFireType::EWFT_None:
		break;

	case EWeaponFireType::EWFT_Hitscan:

		BulletTrace();

		break;

	case EWeaponFireType::EWFT_Projectile:
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
	if (!IsValid(PlayerRef))
		return;

	bIsFiring = false;

	if (!bIsReloading && !bIsFiring && HasAmmoForReload() && !IsMagFull())
	{
		PlayerRef->PlayerReloadWeapon();

		bIsReloading = true;

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
			for (auto&& Result : OutResult)
				CreateImpactFX(Result);
	}
}

void AWeaponBase::ResetFireOrReload(float DeltaTime)
{
	if (!WeaponAnimInstance->Montage_IsPlaying(WeaponFireMontage))
		bIsFiring = false;

	else
		bIsFiring = true;

	if (!WeaponAnimInstance->Montage_IsPlaying(WeaponReloadMontage))
		bIsReloading = false;

	else
		bIsReloading = true;
}

bool AWeaponBase::MagHasAmmo() { return WeapStats.CurrentMagTotal > 0; }

bool AWeaponBase::HasAmmoForReload() { return WeapStats.CurrentTotalAmmo > 0; }

bool AWeaponBase::IsMagFull() { return WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal; }

bool AWeaponBase::CanWeaponFire() { return !bIsFiring && !bIsReloading && WeapStats.CurrentMagTotal > 0; }

bool AWeaponBase::CanWeaponReload() { return !bIsReloading && !bIsFiring && WeapStats.CurrentTotalAmmo > 0 && WeapStats.CurrentMagTotal < WeapStats.MaxMagTotal; }

void AWeaponBase::ShotgunReloadStart() { bIsFiring = false; bIsReloading = true; }

void AWeaponBase::ShotgunReloadLoop() {}

void AWeaponBase::ShotgunReloadEnd() { bIsReloading = false; }

void AWeaponBase::WeaponSetup() {}

void AWeaponBase::StopFire() {}
