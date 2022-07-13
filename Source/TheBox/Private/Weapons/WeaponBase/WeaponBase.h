#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentTotalAmmo, int32, TotalAmmoCount);

UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
#pragma region GETTERS

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE TObjectPtr<class UTexture2D> GetIcon() { return Icon; }
	FORCEINLINE FName GetSocketName() const { return SocketName; }
	FORCEINLINE EWeaponName GetCurrentWeaponEnumName() { return WeaponName; }
	FORCEINLINE EWeaponFireType GetFireType() { return FireType; }
	FORCEINLINE FName GetWeaponName() { return NameOfWeapon; }
	FORCEINLINE bool GetCanFire() { return bCanFire; }
	FORCEINLINE int32 GetWeaponIndex() { return WeaponIndex; }
	FORCEINLINE int32 GetCurrentAmmo() { return CurrentMagTotal; }
	FORCEINLINE int32 GetCurrentTotalAmmo() { return CurrentTotalAmmo; }
	FORCEINLINE int32 GetLowAmmo() { return LowAmmo; }

#pragma endregion

public:	
	// Sets default values for this actor's properties
	AWeaponBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<UAnimInstance> WeaponAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponFireMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enums)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSocketName)
	FName SocketName;

#pragma region STATS

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 CurrentMagTotal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 MaxMagTotal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 CurrentTotalAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 MaxTotalAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 LowAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 CrosshairIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	int32 WeaponIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	float CriciticalHitChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	FName NameOfWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	TObjectPtr<class UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stats)
	EWeaponFireType FireType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	TObjectPtr<class UNiagaraSystem> AmmoEject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	TObjectPtr<class UNiagaraSystem> FireFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	TObjectPtr<class USoundBase> FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	TObjectPtr<class USoundBase> RackSlideSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	TObjectPtr<class USoundBase> MagOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	TObjectPtr<class USoundBase> MagInSound;

#pragma endregion

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
