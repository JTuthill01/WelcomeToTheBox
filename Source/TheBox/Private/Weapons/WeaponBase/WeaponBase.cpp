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

#define CAMERA_FORWARD PlayerRef->GetPlayerCamera()->GetForwardVector()
#define CAMERA_RIGHT PlayerRef->GetPlayerCamera()->GetRightVector()
#define CAMERA_UP PlayerRef->GetPlayerCamera()->GetUpVector()

// Sets default values
AWeaponBase::AWeaponBase() : SocketName(NAME_None), ShotgunPellets(6), Range(4'500), SpreadAngle(22.6F), bCanFire(true), bCanReload(true), bIsReloading(false), bIsFiring(false), EjectQuat(FQuat(0.F)), FireQuat(FQuat(0.F)),
	WeaponFireTimer(0.F), WeaponReloadTimer(0.F), InUintToEnum(0), ShotgunReloadStartIndex(0), ShotgunReloadLoopIndex(1), ShotgunReloadEndIndex(2)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCastShadow(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->bReturnMaterialOnMove = true;

	WeaponParser = CreateDefaultSubobject<UWeaponComponentParser>(TEXT("Weapon Parser"));

	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(WeaponMesh))
		WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponSetup();
}

void AWeaponBase::BulletTrace(FHitResult& HitResult, FTransform& ProjectileTransform)
{
	if (!IsValid(PlayerRef))
		return;

	FVector Start{ PlayerRef->GetPlayerCamera()->GetComponentLocation() };
	FVector Rotation{ PlayerRef->GetPlayerCamera()->GetComponentRotation().Vector() };
	FVector End{ Start + Rotation * 10'000.F };
	FVector Scale = FVector(1.F);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(UGameplayStatics::GetPlayerController(PlayerRef, 0));
	ActorsToIgnore.Add(PlayerRef);
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bHasBeenHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, false);

	FVector Translation = HitResult.Location;
	FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("Fire_FX_Slot"));

	if (bHasBeenHit)
	{
		FRotator TempRotator = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Translation);

		ProjectileTransform = UKismetMathLibrary::MakeTransform(Translation, TempRotator, Scale);
	}

	else
	{
		FRotator TempRotator = PlayerRef->GetController()->GetControlRotation();

		if (TempRotator == FRotator(0.F))
			return;

		else
			ProjectileTransform = UKismetMathLibrary::MakeTransform(SocketLocation, TempRotator, Scale);
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
	bCanFire = false;
	bCanReload = false;
	bIsFiring = true;

	WeapStats.CurrentMagTotal--;

	OnWeaponFire.Broadcast(WeapStats.CurrentMagTotal);

	if (WeapStats.CurrentMagTotal <= 0)
		WeapStats.CurrentMagTotal = 0;

	FTransform ImpactTransform;
	FHitResult ImpactResult;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (WeapStats.FireType)
	{
	case EWeaponFireType::EWFT_None:
		break;

	case EWeaponFireType::EWFT_Hitscan:

		BulletTrace(ImpactResult, ImpactTransform);

		if (ImpactResult.bBlockingHit)
			CreateImpactFX(ImpactResult);

		break;

	case EWeaponFireType::EWFT_Projectile:
		break;

	case EWeaponFireType::EWFT_SpreadScan:

		ShotgunCone();

		break;

	default:
		break;
	}
}

void AWeaponBase::ShotgunCone()
{
	if (!IsValid(PlayerRef))
		return;

	FVector Start{ PlayerRef->GetPlayerCamera()->GetRelativeLocation() };

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(UGameplayStatics::GetPlayerController(PlayerRef, 0));
	ActorsToIgnore.Add(PlayerRef);
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	FHitResult Result;

	for (int32 i = 1; i < ShotgunPellets; ++i)
	{
		float Temp = Range * (UKismetMathLibrary::DegTan(SpreadAngle));

		FVector2D Focii = RandPointInCircle(Temp);

		FVector FociiX{ CAMERA_RIGHT * Focii.X };

		FVector FociiY{ CAMERA_UP * Focii.Y };

		FVector End{ (CAMERA_FORWARD * 10'000.F) + Start + FociiY };

		const bool bTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, Result, true, 
			FLinearColor::FromSRGBColor(FCustomColorsFromHex::ShamrockGreen()));
	}
}

void AWeaponBase::WeaponReload()
{
	bCanFire = false;

	bCanReload = false;

	WeapStats.CurrentTotalAmmo -= WeapStats.MaxMagTotal - WeapStats.CurrentMagTotal;

	WeapStats.CurrentMagTotal = UKismetMathLibrary::Min(WeapStats.MaxMagTotal, WeapStats.CurrentTotalAmmo);

	WeapStats.CurrentMagTotal = FMath::Clamp(WeapStats.CurrentMagTotal, 0, WeapStats.MaxMagTotal);

	WeapStats.CurrentTotalAmmo = FMath::Clamp(WeapStats.CurrentTotalAmmo, 0, WeapStats.MaxTotalAmmo);

	OnWeaponReload.Broadcast(WeapStats.CurrentMagTotal, WeapStats.CurrentTotalAmmo);
}

FVector2D AWeaponBase::RandPointInCircle(float Radius)
{
	float FullCircle = 360.F;
	float Angle = UKismetMathLibrary::RandomFloatInRange(NULL, FullCircle);
	float DistanceFromFocus = UKismetMathLibrary::RandomFloatInRange(NULL, Radius);

	float X = DistanceFromFocus * (UKismetMathLibrary::DegCos(Angle));
	float Y = DistanceFromFocus * (UKismetMathLibrary::DegSin(Angle));

	FVector2D OutResult { X, Y };

	return OutResult;
}

bool AWeaponBase::MagHasAmmo() { return WeapStats.CurrentMagTotal > 0; }

bool AWeaponBase::HasAmmoForReload() { return WeapStats.CurrentTotalAmmo > 0; }

bool AWeaponBase::IsMagFull() { return WeapStats.CurrentMagTotal >= WeapStats.MaxMagTotal; }

void AWeaponBase::ShotgunReloadStart() {}

void AWeaponBase::ShotgunReloadLoop() {}

void AWeaponBase::ShotgunReloadEnd() {}

void AWeaponBase::WeaponSetup() {}

void AWeaponBase::StopFire() {}
