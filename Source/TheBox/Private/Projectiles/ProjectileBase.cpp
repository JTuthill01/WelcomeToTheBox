#include "Projectiles/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"

// Sets default values
AProjectileBase::AProjectileBase() : bIsCausingRadiusDamage(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 10000.F;
	ProjectileMovement->MaxSpeed = 10000.F;
	ProjectileMovement->ProjectileGravityScale = 0.2F;
	ProjectileMovement->bSweepCollision = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(8.F);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->bReturnMaterialOnMove = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(Sphere);

	SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProjectileBase::ProjectileStop);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::ProjectileStop(const FHitResult& HitResult)
{
	Destroy();

	if (bIsCausingRadiusDamage)
		ExplodeOnImpact(HitResult);

	else
		SpawnImpactFX(HitResult);
}

void AProjectileBase::ExplodeOnImpact(const FHitResult& HitResult)
{
	if (!IsValid(PlayerRef->GetCurrentWeapon()))
		return;

	CurrentWeaponStats.DamageRadius = PlayerRef->GetCurrentWeapon()->GetDamageRadius();
	CurrentWeaponStats.DamageAmount = PlayerRef->GetCurrentWeapon()->GetDamageAmount();
	
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	ActorsToIgnore.Add(PlayerRef->GetCurrentWeapon());
	ActorsToIgnore.Add(this);

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	const bool bHasSphereOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), HitResult.ImpactPoint, CurrentWeaponStats.DamageRadius, TraceObjects, nullptr, ActorsToIgnore, OutActors);

	if (bHasSphereOverlapped)
	{
		int32 Count = OutActors.Num();

		TArray<AActor*> Ignore;
		Ignore.Add(this);
		Ignore.Add(PlayerRef->GetCurrentWeapon());

		for (int32 i = 0; i < Count; ++i)
		{
			if (OutActors[i]->GetClass() == APlayerCharacter::StaticClass())
			{
				const bool bWasDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), CurrentWeaponStats.DamageAmount, HitResult.ImpactPoint, CurrentWeaponStats.DamageRadius, UDamageType::StaticClass(),
					Ignore, this, GetInstigatorController(), true, ECollisionChannel::ECC_Pawn);
			}
		}
	}
}

void AProjectileBase::SpawnImpactFX(const FHitResult& HitResult)
{
}

