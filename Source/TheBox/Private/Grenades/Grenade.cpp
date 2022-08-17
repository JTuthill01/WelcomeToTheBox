#include "Grenades/Grenade.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Structs/HexColors/Str_CustomHexColors.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGrenade::AGrenade() : DamageRadius(500.F), DamageAmount(15.F), ExplosionTimer(4.F)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Grenade Mesh"));
	GrenadeMesh->SetCastShadow(false);

	SetRootComponent(GrenadeMesh);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GrenadeMesh))
	{
		GrenadeInstance = GrenadeMesh->GetAnimInstance();

		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AGrenade::Explode, ExplosionTimer, false);
	}
}

void AGrenade::Explode()
{
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);

	UGameplayStatics::SpawnSoundAttached(ExplosionSound, GrenadeMesh);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, GrenadeMesh->GetRelativeLocation(), GrenadeMesh->GetRelativeRotation());

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	const bool bHasSphereOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GrenadeMesh->GetRelativeLocation(), DamageRadius, TraceObjects, nullptr, ActorsToIgnore, OutActors);

	DrawDebugSphere(GetWorld(), GrenadeMesh->GetRelativeLocation(), DamageRadius, 12, FCustomColorsFromHex::PurpleLily(), true, 6.F);

	if (bHasSphereOverlapped)
	{
		for (auto&& Out : OutActors)
			const bool bWasDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GrenadeMesh->GetRelativeLocation(), DamageRadius,
				UDamageType::StaticClass(), ActorsToIgnore, this, GetInstigatorController(), true, ECollisionChannel::ECC_Visibility);
	}

	Destroy();
}

void AGrenade::OnGrenadeThrow(FVector ForwardVector)
{
	ForwardVector *= 2'500.F;

	GrenadeMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GrenadeMesh->AddImpulse(ForwardVector);
}
