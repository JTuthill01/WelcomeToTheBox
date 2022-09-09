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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSwap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadeThrown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrenadePickup);

UCLASS()
class APlayerCharacter : public ACharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	//Sets default values for this character's properties
	APlayerCharacter();

public:

	EWeaponName PreviousWeapon_01;
	EWeaponName PreviousWeapon_02;
	EWeaponName PreviousWeapon_03;
	EWeaponName PreviousWeapon_04;

public:

#pragma region C++ only Getters

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetPlayerArms() const { return Arms; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetPlayerCamera() const { return Camera; }
	FORCEINLINE TObjectPtr<class UPlayerHealthComponent> GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE TObjectPtr<UAnimInstance> GetPlayerAnimInstance() const { return PlayerAnimInstance; }
	FORCEINLINE TMap<EWeaponName, TObjectPtr<class AWeaponBase>> GetWeaponMap() const { return WeaponMap; }

	FORCEINLINE EWeaponSlot GetCurrentSlotIndex() const { return WeaponIndexEnum; }
	FORCEINLINE EWeaponName GetCurrentWeaponSpawnEnum() const { return WeaponSpawnEnum; }

	FORCEINLINE int32 GetWeaponIndex() const { return WeaponIndex; }
	FORCEINLINE int32 GetGrenadeCount() const { return CurrentGrenades; }

	FORCEINLINE bool HasOpenWeaponSlot() const { return bHasOpenSlot; }

#pragma endregion

#pragma region Blueprint and C++ Getters

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE EWeaponName GetCurrentWeaponName() const { return CurrentNameOfWeapon; }

#pragma endregion

	/** Setter for the EWeaponSlot Enum, used in PlayerCharacterController for switching weapons */
	FORCEINLINE void SetWeaponSlotIndex(EWeaponSlot NewValue) { WeaponIndexEnum = NewValue; }

	FORCEINLINE void SetWeaponSpawnEnum(EWeaponName SpawnName) { WeaponSpawnEnum = SpawnName; }

	FORCEINLINE void SetWeaponName(EWeaponName NewName) { CurrentNameOfWeapon = NewName; }

	FORCEINLINE bool HasMaxGrenades() { return CurrentGrenades >= MaxGrenades; }

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
	void ThrowGrenade();

	UFUNCTION()
	void PlayerFireWeapon();

	UFUNCTION()
	void PlayerReloadWeapon();

	UFUNCTION()
	void OnGrenadeReleased();

	UFUNCTION()
	void SetGrenadeCount(int32 NewCount);

	UFUNCTION()
	void SetWeaponVisibility(bool ShouldBeHidden);

	UFUNCTION(BlueprintCallable)
	void SpawnWeaponMap(TSubclassOf<class AWeaponBase> WeaponSub, EWeaponName WeaponEnumName, bool& IsSuccessful);

	UFUNCTION()
	void LoadWeaponBP(FString WeaponNameString);

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

	void SpawnInitialWeapon();

public:
	UPROPERTY(BlueprintAssignable)
	FOnClearViewport Clear;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponSwap OnSwap;

	UPROPERTY(BlueprintAssignable)
	FOnGrenadeThrown OnGrenadeThrown;

	UPROPERTY(BlueprintAssignable)
	FOnGrenadePickup OnGrenadePickup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>ItalianReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>AmericanReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunMontages)
	TArray<TObjectPtr<class UAnimMontage>>BulldogReloadMonatge;

public:

#pragma region Weapon Slots

	UPROPERTY()
	TObjectPtr<class AWeaponBase> WeaponSlot_01;

	UPROPERTY()
	TObjectPtr<class AWeaponBase> WeaponSlot_02;

	UPROPERTY()
	TObjectPtr<class AWeaponBase> WeaponSlot_03;

	UPROPERTY()
	TObjectPtr<class AWeaponBase> WeaponSlot_04;

#pragma endregion

private:

#pragma region Weapon Spawn 

	UPROPERTY()
	TObjectPtr<UClass> LoadedBpAsset;

	UPROPERTY()
	TSoftClassPtr<AActor> ActorBpClass;

	UPROPERTY()
	TObjectPtr<class UBPLoaderComponent> BPLoader;

#pragma endregion

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> PlayerAnimInstance;

	UPROPERTY()
	TObjectPtr<class UTheBoxGameInstance> BoxedInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grenades, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AGrenade> Grenade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealthComp, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponName, TObjectPtr<AWeaponBase>> WeaponMap; /** If this works rename later */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponBase> InitialWeaponToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grenades, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGrenade> GrenadeToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Grenades, meta = (AllowPrivateAccess = "true"))
	FName GrenadeSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponRaiseMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ThrowGrenadeMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colors, meta = (AllowPrivateAccess = "true"))
	FCustomColors UserColors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	int32 MaxGrenades;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	int32 CurrentGrenades;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDVariables, meta = (AllowPrivateAccess = "true"))
	EWeaponName CurrentNameOfWeapon;

private:
	UPROPERTY()
	float InteractableTraceTimer;

	UPROPERTY()
	FTimerHandle InteractableTraceTimerHandle;

private:
	EWeaponSlot WeaponIndexEnum;
	EWeaponName WeaponSpawnEnum;

	int32 WeaponIndex;

	bool bIsFirstSlotFull;
	bool bIsSecondSlotFull;
	bool bIsThirdSlotFull;
	bool bIsFourthSlotFull;
	bool bHasOpenSlot;
};
