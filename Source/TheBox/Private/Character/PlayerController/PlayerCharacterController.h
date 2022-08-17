#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Interfaces/Controller/PlayerControllerInterface.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "PlayerCharacterController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class APlayerCharacterController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()
	
public:
	APlayerCharacterController();

public:
	virtual void SetupInputComponent() override;

	virtual void UpdateRotation(float DeltaTime) override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual APlayerCharacterController* SetControllerRef_Implementation() override;

protected:
	virtual void BeginPlay() override;

protected:

#pragma region INPUT

	//Input Actions//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* WeaponFireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* GrenadeThrowAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* WeaponReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* WeaponSwapAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* PrimairyWeaponSwitchAction;

	//Mapping Contexts//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	int32 BaseMappingPriority = 0;

	//Input Functions//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void Jump();
	void StopJump();
	void Sprint();
	void StopSprinting();
	void FireWeapon();
	void StopFiringWeapon();
	void WeaponReload();
	void ThrowGrenade();
	void SwapWeapon();
	void SwitchPrimairyWeapon();

#pragma endregion

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed, meta = (AllowPrivateAccess = "true"))
	float BaseSprintSpeed;

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<class AWeaponBase> WeaponRef;

private:
	EWeaponSlot IndexEnum;
};
