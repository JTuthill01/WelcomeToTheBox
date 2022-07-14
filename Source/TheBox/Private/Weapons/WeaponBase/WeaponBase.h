#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "Structs/WeaponDataStrings/Str_WeaponStringData.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentTotalAmmo, int32, TotalAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFire, int32, Ammo);

UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
#pragma region GETTERS

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE TObjectPtr<class UTexture2D> GetIcon() { return WeapStats.Icon; }

	FORCEINLINE FName GetSocketName() const { return SocketName; }

	FORCEINLINE EWeaponName GetCurrentWeaponEnumName() { return WeaponName; }
	FORCEINLINE EWeaponFireType GetFireType() { return WeapStats.FireType; }

	FORCEINLINE FName GetWeaponName() { return WeapStats.WeaponName; }

	FORCEINLINE bool GetCanFire() { return bCanFire; }

	FORCEINLINE int32 GetWeaponIndex() { return WeapStats.WeaponIndex; }
	FORCEINLINE int32 GetCurrentAmmo() { return WeapStats.CurrentMagTotal; }
	FORCEINLINE int32 GetCurrentTotalAmmo() { return WeapStats.CurrentTotalAmmo; }
	FORCEINLINE int32 GetLowAmmo() { return WeapStats.LowAmmo; }

#pragma endregion

public:	
	// Sets default values for this actor's properties
	AWeaponBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual void WeaponFire();

	UFUNCTION(BlueprintCallable)
	virtual void WeaponReload();

public:
	bool MagHasAmmo();

	bool HasAmmoForReload();

public:
	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentTotalAmmo NewTotalAmmo;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFire OnWeaponFire;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void WeaponSetup();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<UAnimInstance> WeaponAnimInstance;

	UPROPERTY()
	FWeaponStringData WeaponFilePaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parser)
	TObjectPtr<class UWeaponComponentParser> WeaponParser;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponFireMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponStats)
	FWeaponDataStats WeapStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enums)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSocketName)
	FName SocketName;

protected:
	bool bCanFire;
	bool bCanReload;
	bool bIsReloading;

	FQuat EjectQuat;
	FTransform EjectTransform;

	FQuat FireQuat;
	FTransform FireTransform;

	float WeaponFireTimer;

	FTimerHandle WeaponFireTimerHandle;

private:
	void BulletTrace(FHitResult& HitResult, FTransform& ProjectileTransform);

	void CreateImpactFX(FHitResult HitResult);
};
