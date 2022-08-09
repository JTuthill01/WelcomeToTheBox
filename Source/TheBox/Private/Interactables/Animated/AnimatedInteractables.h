#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "AnimatedInteractables.generated.h"

UCLASS()
class AAnimatedInteractables : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AAnimatedInteractables();

public:
	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	void Open();

	void Spawn();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SKBaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimationAsset> AnimToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> CaseOpenFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickupBase> PickupToSpawn;

private:
	bool bHasBeenOpned;

	float CaseOpenTimer;

	FTimerHandle CaseOpenTimerHandle;
};
