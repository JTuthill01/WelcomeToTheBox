#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Enums/PickupEnums/PickupEnums.h"
#include "AnimatedInteractables.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSpawned, int32, OutGoingValue);

UCLASS()
class AAnimatedInteractables : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AAnimatedInteractables();

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSpawned OnWeaponSpawned;

public:
	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnSetup();

	void Open();

	void Spawn();

	void SetPickupData(EPickupType InType);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APickupBase> TempPickupPtr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SKBaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimationAsset> AnimToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	EPickupType PickupEnum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickupBase> PickupToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	int32 MaxNumToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	int32 PickupIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	bool bIsWeaponContainer;

private:
	bool bHasBeenOpned;

	float CaseOpenTimer;

	FTransform SpawnTransform;

	FTimerHandle CaseOpenTimerHandle;
};
