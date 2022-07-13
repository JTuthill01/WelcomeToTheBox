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

#pragma region Getters
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetPlayerArms() { return Arms; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetPlayerCamera() { return Camera; }
	FORCEINLINE TObjectPtr<class UPlayerHealthComponent> GetHealthComponent() { return HealthComponent; }
	FORCEINLINE TObjectPtr<class AWeaponBase> GetCurrentWeapon() { return CurrentWeapon; }
#pragma endregion

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

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void PlayerDeath();

private:
	void ScanForInteractables();
	void Initialize();

public:
	UPROPERTY(BlueprintAssignable)
	FOnClearViewport Clear;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealthComp, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponName, UAnimMontage*>CharacterFireMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colors, meta = (AllowPrivateAccess = "true"))
	FCustomColors UserColors;

private:
	float InteractableTraceTimer;

	FTimerHandle InteractableTraceTimerHandle;
};
