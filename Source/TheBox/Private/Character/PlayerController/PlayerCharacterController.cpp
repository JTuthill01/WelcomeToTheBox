#include "Character/PlayerController/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerCharacterInterface.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "Misc/CameraManager/TheBoxCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "Structs/HexColors/Str_CustomHexColors.h"

APlayerCharacterController::APlayerCharacterController() : BaseMovementSpeed(600.F), BaseSprintSpeed(800.F)
{
	PlayerCameraManagerClass = ATheBoxCameraManager::StaticClass();
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerCharacterInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerCharacterController::UpdateRotation(float DeltaTime)
{
	Super::UpdateRotation(DeltaTime);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
		Subsystem->ClearAllMappings();

		// Add each mapping context, along with their priority values. Higher values out prioritize lower values.
		Subsystem->AddMappingContext(BaseMappingContext, BaseMappingPriority);
	}

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJump);
		}

		if (SprintAction)
		{
			PlayerEnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacterController::Sprint);
			PlayerEnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopSprinting);
		}

		if (WeaponFireAction)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Started, this, &APlayerCharacterController::FireWeapon);
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopFiringWeapon);
		}

		if (WeaponReloadAction)
			PlayerEnhancedInputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::WeaponReload);

		if (MovementAction)
			PlayerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);

		if (LookAction)
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);

		if (InteractAction)
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Interact);

		if (WeaponSwapAction)
			PlayerEnhancedInputComponent->BindAction(WeaponSwapAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::SwapWeapon);
	}
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.F)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value[1]);
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value[0]);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	AddPitchInput(Value[1] * -1);
	AddYawInput(Value[0]);
}

void APlayerCharacterController::Interact()
{
	if (!IsValid(PlayerRef))
		return;
	
	else
	{
		PlayerRef->Clear.Broadcast();

		PlayerRef->InteractWithObject();
	}
}

void APlayerCharacterController::FireWeapon()
{
	if (!IsValid(PlayerRef) || !IsValid(PlayerRef->GetCurrentWeapon()))
		return;

	if (PlayerRef->GetCurrentWeapon()->CanFireOrReload() && PlayerRef->GetCurrentWeapon()->MagHasAmmo())
	{
		PlayerRef->GetCurrentWeapon()->WeaponFire();

		PlayerRef->PlayerFireWeapon();
	}

	else if (!PlayerRef->GetCurrentWeapon()->MagHasAmmo() && PlayerRef->GetCurrentWeapon()->HasAmmoForReload())
		WeaponReload();

	else
		return;
}

void APlayerCharacterController::WeaponReload()
{
	if (!IsValid(PlayerRef) || !IsValid(PlayerRef->GetCurrentWeapon()))
		return;

	EWeaponType WeaponTypeEnum = PlayerRef->GetCurrentWeapon()->GetWeaponType();
	
	if (WeaponTypeEnum == EWeaponType::EWT_Shotgun)
	{
		if (PlayerRef->GetCurrentWeapon()->CanShotgunFireOrReload())
		{
			PlayerRef->GetCurrentWeapon()->ShotgunReloadStart();
		}
	}

	else if (PlayerRef->GetCurrentWeapon()->CanFireOrReload())
	{
		PlayerRef->GetCurrentWeapon()->WeaponReload();

		PlayerRef->PlayerReloadWeapon();
	}

	else
		return;
}

void APlayerCharacterController::SwapWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.F, FCustomColorsFromHex::PlumVelvet(), __FUNCTION__);
}

void APlayerCharacterController::StopFiringWeapon() { PlayerRef->GetCurrentWeapon()->StopFire(); }

void APlayerCharacterController::Jump() { GetCharacter()->Jump(); }

void APlayerCharacterController::StopJump() { GetCharacter()->StopJumping(); }

void APlayerCharacterController::Sprint() { PlayerRef->GetCharacterMovement()->MaxWalkSpeed = BaseSprintSpeed; }

void APlayerCharacterController::StopSprinting() { PlayerRef->GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed; }

APlayerCharacterController* APlayerCharacterController::SetControllerRef_Implementation() { return this; }

