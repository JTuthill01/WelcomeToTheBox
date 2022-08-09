#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "ProjectileBase.generated.h"

UCLASS()
class AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void ProjectileStop(const FHitResult& HitResult);

protected:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	FWeaponDataStats CurrentWeaponStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionSphere)
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NiagaraFX)
	TObjectPtr<class UNiagaraSystem> ParticleTrail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsCausingRadiusDamage;

private:
	void ExplodeOnImpact(const FHitResult& HitResult);

	void SpawnImpactFX(const FHitResult& HitResult);

	void DealDamage(const FHitResult& HitResult);
};
