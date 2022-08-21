#include "Grenades/Grenade.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenade::AGrenade() : DamageRadius(500.F), DamageAmount(12.F), ExplosionTimer(4.F)
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

		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AGrenade::TypeSwitch, ExplosionTimer, false);
	}
}

void AGrenade::TypeSwitch()
{
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);

	switch (GrenadeType)
	{
	case EGrenadeType::EGT_None:
		break;

	case EGrenadeType::EGT_Frag:

		Explode();

		break;

	case EGrenadeType::EGT_Incendary:
		break;

	case EGrenadeType::EGT_Electric:
		break;

	case EGrenadeType::EGT_FlashBang:
		break;

	case EGrenadeType::EGT_Corrosive:
		break;

	case EGrenadeType::EGT_Smoke:
		break;

	default:
		break;
	}
}

void AGrenade::Explode()
{
	UGameplayStatics::SpawnSoundAttached(ExplosionSound, GrenadeMesh);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, GrenadeMesh->GetRelativeLocation(), GrenadeMesh->GetRelativeRotation());

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	const bool bHasSphereOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GrenadeMesh->GetRelativeLocation(), DamageRadius, TraceObjects, nullptr, ActorsToIgnore, OutActors);

	if (bHasSphereOverlapped)
	{
		for (auto&& Out : OutActors)
			const bool bWasDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GrenadeMesh->GetRelativeLocation(), DamageRadius,
				UDamageType::StaticClass(), ActorsToIgnore, this, GetInstigatorController(), false, ECollisionChannel::ECC_Visibility);
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
