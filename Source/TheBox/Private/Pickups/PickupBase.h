#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/PickupData/Str_PickupData.h"
#include "Structs/WeaponPickup/Str_WeaponPickup.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "Enums/PickupEnums/PickupEnums.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "PickupBase.generated.h"

UCLASS()
class APickupBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	FORCEINLINE void SetPickupWeaponName(EWeaponName NewName) { PickupWeaponName = NewName; }
	FORCEINLINE void SetPickupBaseType(EPickupType Base) { PickupBaseType = Base; }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	FPickupData PickupData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TObjectPtr<UDataTable> PickupDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EPickupType PickupBaseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EPickupHealthType BaseHealthType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EPickupAmmoType PickupAmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EWeaponName PickupWeaponName;

private:
	UFUNCTION()
	void OnClearViewport();

	UFUNCTION()
	void Setup();

private:
	void HealthPickup(EPickupHealthType InHealthType);
	void AmmoPickup(EPickupAmmoType InAmmoType);

	void SetData();
	void SetWeaponPickupData();
	void SetHealthData(EPickupHealthType Health);
	void SetAmmoData(EPickupAmmoType PickupAmmo);
	void SetArmorData();
	void SetWeaponData(EWeaponName Name);

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<class UPickupComponent> PickupParser;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponPickup, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AWeaponBase>> WeaponPickupToSpawn;

private:
	FWeaponPickup WeaponPickupStr;

	float HealthValue;
	float ArmorValue;

	uint8 InName;
};
