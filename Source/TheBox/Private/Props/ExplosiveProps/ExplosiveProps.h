#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveProps.generated.h"

UCLASS()
class AExplosiveProps : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveProps();

public:
	UFUNCTION()
	void PropTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void DealRadialDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void ChangePropHealth(float Damage);

private:
	void ExplodeOnDestruction();

	void Cleanup();

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PropMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ExplosionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> ExplosiveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	FName ExplosivePropSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ExplosionSettings, meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ExplosionSettings, meta = (AllowPrivateAccess = "true"))
	float DamageAmount;

private:
	float PropHealth;
	float RemovalTimer;

	FTimerHandle RemovalTimerHandle;
};
