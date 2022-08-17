#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "Structs/WeaponDataStrings/Str_WeaponStringData.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentTotalAmmo, int32, TotalAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFire, int32, Ammo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponReload, int32, CurrentMagAmount, int32, CurrentTotalAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShotgunReload, int32, CurrentAmount, int32, CurrentTotal);

UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
#pragma region GETTERS

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE TObjectPtr<class UTexture2D> GetIcon() { return WeapStats.Icon; }
	FORCEINLINE TObjectPtr<class USoundBase> GetMagTapSound() { return MagTapSound; }

	FORCEINLINE FName GetSocketName() const { return SocketName; }

	FORCEINLINE EWeaponName GetCurrentWeaponEnumName() { return WeaponName; }
	FORCEINLINE EWeaponFireType GetFireType() { return WeapStats.FireType; }
	FORCEINLINE EWeaponType GetWeaponType() { return WeapStats.Type; }

	FORCEINLINE float GetDamageRadius() { return WeapStats.DamageRadius; }
	FORCEINLINE float GetDamageAmount() { return WeapStats.DamageAmount; }

	FORCEINLINE int32 GetWeaponIndex() { return WeapStats.WeaponIndex; }
	FORCEINLINE int32 GetCurrentAmmo() { return WeapStats.CurrentMagTotal; }
	FORCEINLINE int32 GetCurrentTotalAmmo() { return WeapStats.CurrentTotalAmmo; }
	FORCEINLINE int32 GetLowAmmo() { return WeapStats.LowAmmo; }

#pragma endregion

	FORCEINLINE void SetCanShgotgunFireOrReload(bool bReset) { bCanShotgunFireOrReload = bReset; }
	FORCEINLINE void SetWeaponName(EWeaponName NewName) { WeaponName = NewName; }

#pragma region Blueprint Getters

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FName GetWeaponName() { return WeapStats.WeaponName; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetWeaponIndexBP() { return WeapStats.WeaponIndex; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCrosshairIndex() { return WeapStats.CrosshairIndex; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	EWeaponName GetCurrentWeaponEnumNameBP() { return WeaponName; }

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

	UFUNCTION()
	virtual void ShotgunReloadStart();

	void SetTotalAmmo(int32 NewAmmoValue);

public:
	bool MagHasAmmo();

	bool HasAmmoForReload();

	bool IsMagFull();

	bool CanFireOrReload();

	bool IsAmmoFull();

	bool CanShotgunFireOrReload();

	bool IsWeaponFiringOrReloading();

public:
	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentTotalAmmo OnNewTotalAmmo;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFire OnWeaponFire;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponReload OnWeaponReload;

	UPROPERTY(BlueprintAssignable)
	FOnShotgunReload OnShotgunReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponStats)
	FWeaponDataStats WeapStats;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void WeaponSetup();

	UFUNCTION()
	virtual void ShotgunReloadLoop();

	UFUNCTION()
	virtual void ShotgunReloadEnd();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<class AProjectileBase> Projectile;

	UPROPERTY()
	TObjectPtr<UAnimInstance> WeaponAnimInstance;

	UPROPERTY()
	FWeaponStringData WeaponFilePaths;

	UPROPERTY()
	FTimerHandle ShotgunReloadTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponRoot)
	TObjectPtr<class USceneComponent> WeaponRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parser)
	TObjectPtr<class UWeaponComponentParser> WeaponParser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundBase> MagTapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AProjectileBase> ProjectileToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enums)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSocketName)
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	int32 ShotgunPellets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	int32 Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	float SpreadAngle;

protected:
	bool bHasHitOccured;
	bool bCanShotgunFireOrReload;

	FQuat EjectQuat;
	FTransform EjectTransform;

	FQuat FireQuat;
	FTransform FireTransform;

	uint8 InUintToEnum;
	uint8 InType;

	uint8 ShotgunReloadStartIndex;
	uint8 ShotgunReloadLoopIndex;
	uint8 ShotgunReloadEndIndex;

private:
	void BulletTrace();

	void CreateImpactFX(FHitResult HitResult);

	void ShotgunFireMulti(int32 InShotgunPelletCount);

	void SpawnProjectile();

	void InitializeVariables();
};
