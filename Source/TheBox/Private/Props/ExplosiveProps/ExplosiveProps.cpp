#include "Props/ExplosiveProps/ExplosiveProps.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/PlayerCharacter.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include <Structs/HexColors/Str_CustomHexColors.h>

// Sets default values
AExplosiveProps::AExplosiveProps() : PropHealth(100.F), DamageRadius(550.F), DamageAmount(10.F), RemovalTimer(0.3F)
{
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Prop");
	PropMesh->SetCastShadow(false);
	PropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PropMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PropMesh->SetSimulatePhysics(true);
	PropMesh->SetEnableGravity(true);
	PropMesh->SetGenerateOverlapEvents(true);
	PropMesh->bReturnMaterialOnMove = true;

	SetRootComponent(PropMesh);
}

// Called when the game starts or when spawned
void AExplosiveProps::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AExplosiveProps::PropTakeDamage);
}

void AExplosiveProps::PropTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ChangePropHealth(Damage);
}

void AExplosiveProps::DealRadialDamage()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	const bool bHasSphereOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PropMesh->GetRelativeLocation(), DamageRadius, TraceObjects, nullptr, ActorsToIgnore, OutActors);

	if (bHasSphereOverlapped)
	{
		int32 Count = OutActors.Num();

		for (int32 i = 0; i < Count; ++i)
			const bool bWasDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, PropMesh->GetRelativeLocation(), DamageRadius, UDamageType::StaticClass(),
				ActorsToIgnore, this, GetInstigatorController(), false, ECollisionChannel::ECC_Visibility);
	}
}

void AExplosiveProps::ChangePropHealth(float Damage)
{
	if (PropHealth > 0.F)
	{
		PropHealth -= Damage;

		if (PropHealth <= 0.F)
		{
			PropHealth = FMath::Clamp(PropHealth, 0, 100.F);

			ExplodeOnDestruction();

			DealRadialDamage();
		}
	}
}

void AExplosiveProps::ExplodeOnDestruction()
{
	PropMesh->SetHiddenInGame(true);

	FTransform FXTransform = PropMesh->GetSocketTransform(ExplosivePropSocketName);

	FQuat FXQuat = FXTransform.GetRotation();

	UGameplayStatics::SpawnSoundAttached(ExplosiveSound, PropMesh, ExplosivePropSocketName);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, FXTransform.GetTranslation(), FXQuat.Rotator());

	GetWorldTimerManager().SetTimer(RemovalTimerHandle, this, &AExplosiveProps::Cleanup, RemovalTimer, false);
}

void AExplosiveProps::Cleanup()
{
	OnTakeAnyDamage.RemoveDynamic(this, &AExplosiveProps::PropTakeDamage);
	
	GetWorldTimerManager().ClearTimer(RemovalTimerHandle);

	Destroy();
}

