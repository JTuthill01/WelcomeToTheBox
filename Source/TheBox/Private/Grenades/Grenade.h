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
	TObjectPtr<UAnimMontage> GetGrenadeMontage() { return ThrowMontage; }
	TObjectPtr<UAnimInstance> GetGrenadeInstance() { return GrenadeInstance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GrenadeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ThrowMontage;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> GrenadeInstance;

private:
	int32 CurrentNumGrenades;
	int32 MaxNumGrenades;
};
