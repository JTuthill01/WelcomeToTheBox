#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/PickupData/Str_PickupData.h"
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

private:
	UFUNCTION()
	void OnClearViewport();

	UFUNCTION()
	void Setup();

private:
	void HealthPickup(EPickupHealthType InHealthType);
	void AmmoPickup(EPickupAmmoType InAmmoType);

	void SetData();
	void SetHealthData(EPickupHealthType Health);
	void SetAmmoData(EPickupAmmoType PickupAmmo);
	void SetArmorData();

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<class UPickupComponent> PickupParser;

private:
	float HealthValue;
	float ArmorValue;

	uint8 InName;
};
