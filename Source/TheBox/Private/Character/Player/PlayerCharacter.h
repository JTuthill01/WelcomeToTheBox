#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Structs/CustomColors/Str_CustomColors.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearViewport);

UCLASS()
class APlayerCharacter : public ACharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	//Sets default values for this character's properties
	APlayerCharacter();

public:

#pragma region C++ only Getters

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetPlayerArms() { return Arms; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetPlayerCamera() { return Camera; }
	FORCEINLINE TObjectPtr<class UPlayerHealthComponent> GetHealthComponent() { return HealthComponent; }
	FORCEINLINE TObjectPtr<UAnimInstance> GetPlayerAnimInstance() { return PlayerAnimInstance; }

	FORCEINLINE EWeaponSlot GetEquippedWeaponIndexEnum() { return WeaponIndexEnum; }

	FORCEINLINE bool HasOpenWeaponSlot() { return bHasOpenSlot; }

#pragma endregion

#pragma region Blueprint and C++ Getters

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE TArray<AWeaponBase*> GetWeaponSlotArray() { return WeaponSlotArray; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE class AWeaponBase* GetCurrentWeapon() { return CurrentWeapon; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE uint8 GetEquippedWeaponIndexUint() { return WeaponIndex = static_cast<uint8>(WeaponIndexEnum); }

#pragma endregion

	/* Setter for the EWeaponSlot Enum, used in PlayerCharacterController for switching weapons */
	FORCEINLINE void SetWeaponSlotIndex(EWeaponSlot NewValue) { WeaponIndexEnum = NewValue; }

public:	
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Player Death
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Interface function for setting Player Ref throughout code
	APlayerCharacter* SetPlayerRef_Implementation() override;

public:
	UFUNCTION()
	void InteractWithObject();

	UFUNCTION()
	void PlayerFireWeapon();

	UFUNCTION()
	void PlayerReloadWeapon();

public:
	void SpawnWeaponFromPickup(TObjectPtr<class AWeaponBase> WeaponRef, TSubclassOf<class AWeaponBase> SpawnRef, bool& IsSuccessful);

	void SwapWeaponFromPickup(TObjectPtr<class AWeaponBase> WeaponRef);

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void PlayerDeath();

	UFUNCTION()
	void ScanForInteractables();

private:
	void Initialize();

public:
	UPROPERTY(BlueprintAssignable)
	FOnClearViewport Clear;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>ItalianReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>AmericanReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>BulldogReloadMonatge;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> PlayerAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealthComp, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AWeaponBase>> WeaponSlotArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponBase> InitialWeaponToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponRaiseMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colors, meta = (AllowPrivateAccess = "true"))
	FCustomColors UserColors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmoHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmoHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	EWeaponName CurrentNameOfWeapon;

private:
	void SpawnInitialWeapon();

private:
	UPROPERTY()
	float InteractableTraceTimer;

	UPROPERTY()
	FTimerHandle InteractableTraceTimerHandle;

private:
	EWeaponSlot WeaponIndexEnum;

	uint8 MaxSlots;
	uint8 WeaponIndex;

	bool bIsDefaultSlotFull;
	bool bIsFirstSlotFull;
	bool bIsSecondSlotFull;
	bool bIsThirdSlotFull;
	bool bHasOpenSlot;
};
