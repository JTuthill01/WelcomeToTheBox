#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

public:
	/** Getters */
	FORCEINLINE TObjectPtr<UAnimMontage> GetGrenadeMontage() { return ThrowMontage; }
	FORCEINLINE TObjectPtr<UAnimInstance> GetGrenadeInstance() { return GrenadeInstance; }

public:
	void OnGrenadeThrow(FVector ForwardVector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Explode();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GrenadeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ThrowMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ExplosionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = "true"))
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = "true"))
	float DamageAmount;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> GrenadeInstance;

private:
	float ExplosionTimer;

	FTimerHandle ExplosionTimerHandle;
};
