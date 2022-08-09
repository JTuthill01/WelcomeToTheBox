#include "Character/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structs/HexColors/Str_CustomHexColors.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : InteractableTraceTimer(0.25F), WeaponIndexEnum(EWeaponSlot::EWS_Default_Slot), MaxSlots(5), WeaponIndex(0), bIsFirstSlotFull(false),
	bIsSecondSlotFull(false), bIsThirdSlotFull(false), bIsFourthSlotFull(false), bHasOpenSlot(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(Camera);
	Arms->SetCastShadow(false);

	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::Initialize()
{
	if (IsValid(Arms))
		PlayerAnimInstance = Arms->GetAnimInstance();

	GetWorldTimerManager().SetTimer(InteractableTraceTimerHandle, this, &APlayerCharacter::ScanForInteractables, InteractableTraceTimer, true);

	HealthComponent->PlayerDeath.AddDynamic(this, &APlayerCharacter::PlayerDeath);

	SpawnInitialWeapon();
}

void APlayerCharacter::SpawnInitialWeapon()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();

	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(InitialWeaponToSpawn, Location, Rotation, Params);

	WeaponSlotArray.Reserve(MaxSlots);

	if (IsValid(CurrentWeapon))
	{
		WeaponIndex = 0;

		//WeaponSlotArray.Insert(CurrentWeapon, 0);

		WeaponSlotArray.AddUnique(CurrentWeapon);

		WeaponSlotArray[WeaponIndex]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponSlotArray[WeaponIndex]->GetSocketName());

		CurrentAmmoHUD = WeaponSlotArray[WeaponIndex]->GetCurrentAmmo();

		MaxAmmoHUD = WeaponSlotArray[WeaponIndex]->GetCurrentTotalAmmo();

		CurrentNameOfWeapon = WeaponSlotArray[WeaponIndex]->GetCurrentWeaponEnumName();

		WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

		WeaponIndex = static_cast<uint8>(WeaponIndexEnum);

		bIsFirstSlotFull = true;
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::PlayerDeath()
{
	/* Temp, only for debugging */
	EndPlay(EEndPlayReason::Destroyed);
}

void APlayerCharacter::ScanForInteractables()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(WeaponSlotArray[WeaponIndex]);

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bIsInteractable = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bIsInteractable)
	{
		if (HitResult.GetActor())
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
				IInteractInterface::Execute_InteractableFound(HitResult.GetActor());
		}
	}

	else 
		Clear.Broadcast();
}

void APlayerCharacter::InteractWithObject()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(WeaponSlotArray[WeaponIndex]);

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bCanInteract = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bCanInteract)
	{
		if (HitResult.GetActor())
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
				IInteractInterface::Execute_InteractWithObject(HitResult.GetActor());
		}
	}
}

void APlayerCharacter::PlayerFireWeapon()
{
	int32 LocalIndex = WeaponSlotArray[GetEquippedWeaponIndexUint()]->GetWeaponIndex();

	if (PlayerWeaponFireMontage.IsValidIndex(LocalIndex))
		PlayerAnimInstance->Montage_Play(PlayerWeaponFireMontage[LocalIndex]);

	else
		return;
}

void APlayerCharacter::PlayerReloadWeapon()
{
	int32 LocalIndex = WeaponSlotArray[GetEquippedWeaponIndexUint()]->GetWeaponIndex();

	EWeaponType LocalWeaponType = WeaponSlotArray[GetEquippedWeaponIndexUint()]->GetWeaponType();

	if (LocalWeaponType != EWeaponType::EWT_Shotgun)
	{
		if (PlayerWeaponReloadMontage.IsValidIndex(LocalIndex))
			PlayerAnimInstance->Montage_Play(PlayerWeaponReloadMontage[LocalIndex]);

		else
			return;
	}

	else
		return;
}

void APlayerCharacter::SpawnWeapon(TSubclassOf<class AWeaponBase> SpawnRef, bool& IsSuccessful)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();

	switch (WeaponIndexEnum)
	{
	case EWeaponSlot::EWS_First_Slot:

		if (!bIsFirstSlotFull)
		{
			WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(SpawnRef, Location, Rotation, Params);

			if (IsValid(WeaponSlot_01))
			{
				WeaponSlot_01->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_01->GetSocketName());

				bIsFirstSlotFull = true;

				WeaponSlotArray.AddUnique(WeaponSlot_01);

				WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

				IsSuccessful = true;

				break;
			}

			else
			{
				IsSuccessful = false;

				break;
			}
		}

		else if (!bIsSecondSlotFull)
		{
			WeaponIndexEnum = EWeaponSlot::EWS_Second_Slot;

			SpawnWeapon(SpawnRef, IsSuccessful);

			break;
		}

		else
		{
			IsSuccessful = false;

			WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

			bHasOpenSlot = false;

			break;
		}

		break;

	case EWeaponSlot::EWS_Second_Slot:

		if (!bIsSecondSlotFull)
		{
			WeaponSlot_02 = GetWorld()->SpawnActor<AWeaponBase>(SpawnRef, Location, Rotation, Params);

			if (IsValid(WeaponSlot_02))
			{
				WeaponSlot_02->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_02->GetSocketName());

				GEngine->AddOnScreenDebugMessage(-1, 6.F, FCustomColorsFromHex::HazelGreen(), L"Ref IsValid");

				bIsSecondSlotFull = true;

				WeaponSlotArray.AddUnique(WeaponSlot_02);

				WeaponIndexEnum = EWeaponSlot::EWS_Second_Slot;

				IsSuccessful = true;

				break;
			}

			else
			{
				IsSuccessful = false;

				WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

				break;
			}
		}

		else if (!bIsThirdSlotFull)
		{
			WeaponIndexEnum = EWeaponSlot::EWS_Third_Slot;

			SpawnWeapon(SpawnRef, IsSuccessful);

			break;
		}

		else
		{
			IsSuccessful = false;

			bHasOpenSlot = false;

			break;
		}

		break;

	case EWeaponSlot::EWS_Third_Slot:

		if (!bIsThirdSlotFull)
		{
			WeaponSlot_03 = GetWorld()->SpawnActor<AWeaponBase>(SpawnRef, Location, Rotation, Params);

			if (IsValid(WeaponSlot_03))
			{
				WeaponSlot_03->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_03->GetSocketName());

				bIsThirdSlotFull = true;

				WeaponSlotArray.AddUnique(WeaponSlot_03);

				WeaponIndexEnum = EWeaponSlot::EWS_Third_Slot;

				IsSuccessful = true;
			}

			else
			{
				IsSuccessful = false;

				break;
			}
		}

		else if (!bIsFourthSlotFull)
		{
			WeaponIndexEnum = EWeaponSlot::EWS_Fourth_Slot;

			SpawnWeapon(SpawnRef, IsSuccessful);
		}

		else
		{
			IsSuccessful = false;

			bHasOpenSlot = false;
		}

		break;

	case EWeaponSlot::EWS_Fourth_Slot:

		if (!bIsFourthSlotFull)
		{
			WeaponSlot_04 = GetWorld()->SpawnActor<AWeaponBase>(SpawnRef, Location, Rotation, Params);

			if (IsValid(WeaponSlot_04))
			{
				WeaponSlot_04->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_04->GetSocketName());

				bIsFourthSlotFull = true;

				WeaponSlotArray.AddUnique(WeaponSlot_04);

				WeaponIndexEnum = EWeaponSlot::EWS_Fourth_Slot;

				IsSuccessful = true;
			}

			else
			{
				IsSuccessful = false;

				break;
			}
		}

		else if (!bIsFirstSlotFull)
		{
			WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

			SpawnWeapon(SpawnRef, IsSuccessful);
		}

		else
		{
			IsSuccessful = false;

			bHasOpenSlot = false;
		}

		break;

	case EWeaponSlot::EWS_Default_Slot:
		break;

	default:
		break;
	}
}

APlayerCharacter* APlayerCharacter::SetPlayerRef_Implementation() { return this; }


