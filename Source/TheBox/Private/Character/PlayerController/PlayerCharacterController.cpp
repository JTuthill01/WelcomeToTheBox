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
#include "Structs/HexColors/Str_CustomHexColors.h"

APlayerCharacterController::APlayerCharacterController() : BaseMovementSpeed(600.F), BaseSprintSpeed(800.F), IndexEnum(EWeaponSlot::EWS_First_Slot)
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

		if (PrimairyWeaponSwitchAction)
			PlayerEnhancedInputComponent->BindAction(PrimairyWeaponSwitchAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::SwitchPrimairyWeapon);

		if (GrenadeThrowAction)
			PlayerEnhancedInputComponent->BindAction(GrenadeThrowAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::ThrowGrenade);

		if (WidgetAction)
			PlayerEnhancedInputComponent->BindAction(WidgetAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::WidgetsStuff);

		if (CameraAction)
			PlayerEnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::SwitchCamera);
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
	if (!IsValid(PlayerRef))
		return;

	EWeaponName TempName = PlayerRef->GetCurrentWeaponName();

	if (PlayerRef->GetWeaponMap()[TempName]->CanFireOrReload() && PlayerRef->GetWeaponMap()[TempName]->MagHasAmmo())
	{
		PlayerRef->GetWeaponMap()[TempName]->WeaponFire();

		PlayerRef->PlayerFireWeapon();
	}

	else if (!PlayerRef->GetWeaponMap()[TempName]->MagHasAmmo() && PlayerRef->GetWeaponMap()[TempName]->HasAmmoForReload())
		WeaponReload();

	else
		return;
}

void APlayerCharacterController::WeaponReload()
{
	if (!IsValid(PlayerRef))
		return;

	EWeaponName TempName = PlayerRef->GetCurrentWeaponName();

	EWeaponType WeaponTypeEnum = PlayerRef->GetWeaponMap()[TempName]->GetWeaponType();
	
	if (WeaponTypeEnum == EWeaponType::EWT_Shotgun)
	{
		if (PlayerRef->GetWeaponMap()[TempName]->CanShotgunFireOrReload())
		{
			PlayerRef->GetWeaponMap()[TempName]->ShotgunReloadStart();
		}
	}

	else if (PlayerRef->GetWeaponMap()[TempName]->CanFireOrReload())
	{
		PlayerRef->GetWeaponMap()[TempName]->WeaponReload();

		PlayerRef->PlayerReloadWeapon();
	}

	else
		return;
}

void APlayerCharacterController::ThrowGrenade()
{
	if (!PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->IsWeaponFiringOrReloading())
		PlayerRef->ThrowGrenade();

	else
		return;
}

void APlayerCharacterController::SwapWeapon()
{
	
}

void APlayerCharacterController::SwitchPrimairyWeapon()
{
	uint8 Num = PlayerRef->GetWeaponMap().Num();

	if (Num == 2)
	{
		PlayerRef->GetWeaponMap()[PlayerRef->WeaponSlot_01->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(true);

		PlayerRef->GetWeaponMap()[PlayerRef->PreviousWeapon_02]->SetActorHiddenInGame(false);

		PlayerRef->SetWeaponName(PlayerRef->PreviousWeapon_02);
	}

	if (Num == 3)
	{
		PlayerRef->GetWeaponMap()[PlayerRef->WeaponSlot_03->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(true);

		PlayerRef->GetWeaponMap()[PlayerRef->PreviousWeapon_01]->SetActorHiddenInGame(false);

		PlayerRef->SetWeaponName(PlayerRef->PreviousWeapon_01);

		PlayerRef->OnSwap.Broadcast();
	}

	else if (Num == 4)
	{
		PlayerRef->GetWeaponMap()[PlayerRef->WeaponSlot_04->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(true);

		PlayerRef->GetWeaponMap()[PlayerRef->PreviousWeapon_01]->SetActorHiddenInGame(false);

		PlayerRef->SetWeaponName(PlayerRef->PreviousWeapon_01);
	}
}

void APlayerCharacterController::SwitchCamera()
{
	/*PlayerRef->GetPlayerCamera()->SetActive(false);

	PlayerRef->GetCamera1()->SetActive(true);*/

	GEngine->AddOnScreenDebugMessage(-1, 6.F, FColor::Purple, __FUNCTION__);
}

void APlayerCharacterController::WidgetsStuff() { OnActivateWidget.Broadcast(); }

void APlayerCharacterController::StopFiringWeapon() { PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->StopFire(); }

void APlayerCharacterController::Jump() { GetCharacter()->Jump(); }

void APlayerCharacterController::StopJump() { GetCharacter()->StopJumping(); }

void APlayerCharacterController::Sprint() { PlayerRef->GetCharacterMovement()->MaxWalkSpeed = BaseSprintSpeed; }

void APlayerCharacterController::StopSprinting() { PlayerRef->GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed; }

APlayerCharacterController* APlayerCharacterController::SetControllerRef_Implementation() { return this; }

