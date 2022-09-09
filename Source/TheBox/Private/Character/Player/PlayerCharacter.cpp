#include "Character/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Grenades/Grenade.h"
#include "Structs/HexColors/Str_CustomHexColors.h"
#include "Kismet/KismetMathLibrary.h"
#include "JsonComponents/BPLoader/BPLoaderComponent.h"
#include "Misc/GameInstance/TheBoxGameInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : MaxGrenades(4), CurrentGrenades(MaxGrenades), InteractableTraceTimer(0.25F), WeaponIndexEnum(EWeaponSlot::EWS_Default_Slot), WeaponIndex(0), bIsFirstSlotFull(false),
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
	BPLoader = CreateDefaultSubobject<UBPLoaderComponent>(TEXT("BP File Loader"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BoxedInstance = Cast<UTheBoxGameInstance>(GetGameInstance());

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

	LoadWeaponBP("L86");

	SpawnInitialWeapon();
}

void APlayerCharacter::SpawnInitialWeapon()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();
	FVector Scale{ 1.F };

	FTransform LocalTransform = UKismetMathLibrary::MakeTransform(Location, Rotation, Scale);

	WeaponMap.Reserve(4);

	WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(LoadedBpAsset, LocalTransform);

	if (IsValid(WeaponSlot_01))
	{
		WeaponMap.Emplace(WeaponSlot_01->GetCurrentWeaponEnumName(), WeaponSlot_01);

		WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->GetSocketName());

		CurrentNameOfWeapon = WeaponSlot_01->GetCurrentWeaponEnumName();

		PreviousWeapon_01 = WeaponSlot_01->GetCurrentWeaponEnumName();

		WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->GetCurrentAmmo();

		WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->GetCurrentTotalAmmo();

		WeaponIndexEnum = EWeaponSlot::EWS_Second_Slot;

		bIsFirstSlotFull = true;
	}
}

void APlayerCharacter::LoadWeaponBP(FString WeaponNameString)
{
	FString ReturnPath = FString("");

	if (IsValid(BoxedInstance))
		ReturnPath = BoxedInstance->LoadParser(WeaponNameString);

	ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(ReturnPath));

	LoadedBpAsset = ActorBpClass.LoadSynchronous();
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
	ActorsToIgnore.Add(WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]);

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
	ActorsToIgnore.Add(WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]);

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
	int32 LocalIndex = WeaponMap[CurrentNameOfWeapon]->GetWeaponIndex();

	if (PlayerWeaponFireMontage.IsValidIndex(LocalIndex))
		PlayerAnimInstance->Montage_Play(PlayerWeaponFireMontage[LocalIndex]);

	else
		return;
}

void APlayerCharacter::PlayerReloadWeapon()
{
	int32 LocalIndex = WeaponMap[CurrentNameOfWeapon]->GetWeaponIndex();

	EWeaponType LocalWeaponType = WeaponMap[CurrentNameOfWeapon]->GetWeaponType();

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

void APlayerCharacter::SpawnWeaponMap(TSubclassOf<class AWeaponBase> WeaponSub, EWeaponName WeaponEnumName, bool& IsSuccessful)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();
	FVector Scale{ 1.F };

	FTransform LocalTransform = UKismetMathLibrary::MakeTransform(Location, Rotation, Scale);

	switch (WeaponIndexEnum)
	{
	case EWeaponSlot::EWS_First_Slot:

		if (!bIsFirstSlotFull)
		{
			WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

			if (IsValid(WeaponSlot_01))
			{
				WeaponMap.Emplace(WeaponSlot_01->GetCurrentWeaponEnumName(), WeaponSlot_01);

				WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponMap[WeaponSlot_01->GetCurrentWeaponEnumName()]->GetSocketName());

				bIsFirstSlotFull = true;

				WeaponIndexEnum = EWeaponSlot::EWS_First_Slot;

				PreviousWeapon_01 = WeaponSlot_01->GetCurrentWeaponEnumName();

				OnSwap.Broadcast();

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

			SpawnWeaponMap(WeaponSub, WeaponEnumName, IsSuccessful);

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
			WeaponSlot_02 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

			if (IsValid(WeaponSlot_02))
			{
				//WeaponMap.Emplace(WeaponSlot_02->GetCurrentWeaponEnumName(), WeaponSlot_02);

				WeaponMap.Emplace(WeaponEnumName, WeaponSlot_02);

				WeaponMap[WeaponSlot_02->GetCurrentWeaponEnumName()]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponMap[WeaponSlot_02->GetCurrentWeaponEnumName()]->GetSocketName());

				CurrentNameOfWeapon = WeaponSlot_02->GetCurrentWeaponEnumName();

				CurrentNameOfWeapon = WeaponEnumName;

				bIsSecondSlotFull = true;

				WeaponIndexEnum = EWeaponSlot::EWS_Third_Slot;

				WeaponMap[PreviousWeapon_01]->SetActorHiddenInGame(true);

				WeaponMap[WeaponSlot_02->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(false);

				PreviousWeapon_02 = WeaponSlot_02->GetCurrentWeaponEnumName();

				OnSwap.Broadcast();

				IsSuccessful = true;

				break;
			}

			else
			{
				IsSuccessful = false;

				break;
			}
		}

		else if (!bIsThirdSlotFull)
		{
			WeaponIndexEnum = EWeaponSlot::EWS_Third_Slot;

			SpawnWeaponMap(WeaponSub, WeaponEnumName, IsSuccessful);

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
			WeaponSlot_03 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

			if (IsValid(WeaponSlot_03))
			{
				//WeaponMap.Emplace(WeaponSlot_03->GetCurrentWeaponEnumName(), WeaponSlot_03);

				WeaponMap.Emplace(WeaponEnumName, WeaponSlot_03);

				WeaponMap[WeaponSlot_03->GetCurrentWeaponEnumName()]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponMap[WeaponSlot_03->GetCurrentWeaponEnumName()]->GetSocketName());

				CurrentNameOfWeapon = WeaponSlot_03->GetCurrentWeaponEnumName();

				bIsThirdSlotFull = true;

				WeaponIndexEnum = EWeaponSlot::EWS_Third_Slot;

				IsSuccessful = true;

				WeaponMap[PreviousWeapon_02]->SetActorHiddenInGame(true);

				WeaponMap[WeaponSlot_03->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(false);

				PreviousWeapon_03 = WeaponSlot_03->GetCurrentWeaponEnumName();

				OnSwap.Broadcast();

				break;
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

			SpawnWeaponMap(WeaponSub, WeaponEnumName, IsSuccessful);

			break;
		}

		else
		{
			IsSuccessful = false;

			bHasOpenSlot = false;

			break;
		}

		break;

	case EWeaponSlot::EWS_Fourth_Slot:

		if (!bIsFourthSlotFull)
		{
			WeaponSlot_04 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

			if (IsValid(WeaponSlot_04))
			{
				//WeaponMap.Emplace(WeaponSlot_04->GetCurrentWeaponEnumName(), WeaponSlot_04);

				WeaponMap.Emplace(WeaponEnumName, WeaponSlot_04);

				WeaponMap[WeaponSlot_04->GetCurrentWeaponEnumName()]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponMap[WeaponSlot_04->GetCurrentWeaponEnumName()]->GetSocketName());

				CurrentNameOfWeapon = WeaponSlot_04->GetCurrentWeaponEnumName();

				bIsFourthSlotFull = true;

				WeaponIndexEnum = EWeaponSlot::EWS_Fourth_Slot;

				WeaponMap[PreviousWeapon_03]->SetActorHiddenInGame(true);

				WeaponMap[WeaponSlot_04->GetCurrentWeaponEnumName()]->SetActorHiddenInGame(false);

				PreviousWeapon_04 = WeaponSlot_04->GetCurrentWeaponEnumName();

				OnSwap.Broadcast();

				IsSuccessful = true;

				break;
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

			SpawnWeaponMap(WeaponSub, WeaponEnumName, IsSuccessful);

			break;
		}

		else
		{
			IsSuccessful = false;

			bHasOpenSlot = false;

			break;
		}

		break;

	case EWeaponSlot::EWS_Default_Slot:
		break;

	default:
		break;
	}
}

void APlayerCharacter::SetGrenadeCount(int32 NewCount)
{
	if (CurrentGrenades < MaxGrenades)
	{
		CurrentGrenades += NewCount;

		if (CurrentGrenades > MaxGrenades)
			CurrentGrenades = FMath::Clamp(CurrentGrenades, NewCount, MaxGrenades);

		OnGrenadePickup.Broadcast();
	}

	else if (CurrentGrenades == MaxGrenades)
		return;
}

void APlayerCharacter::ThrowGrenade()
{
	if (CurrentGrenades <= 0)
	{
		CurrentGrenades = FMath::Clamp(CurrentGrenades, 0, MaxGrenades);

		return;
	}

	else if (CurrentGrenades >= 1)
	{
		Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeToSpawn);

		if (IsValid(Grenade))
		{
			CurrentGrenades--;

			OnGrenadeThrown.Broadcast();

			Grenade->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, GrenadeSocketName);

			Grenade->GetGrenadeInstance()->Montage_Play(Grenade->GetGrenadeMontage());

			PlayerAnimInstance->Montage_Play(ThrowGrenadeMontage);
		}

		else
			return;
	}
}

void APlayerCharacter::OnGrenadeReleased()
{
	if (IsValid(Grenade))
		Grenade->OnGrenadeThrow(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
}

void APlayerCharacter::SetWeaponVisibility(bool ShouldBeHidden) { WeaponMap[CurrentNameOfWeapon]->SetActorHiddenInGame(ShouldBeHidden); }

APlayerCharacter* APlayerCharacter::SetPlayerRef_Implementation() { return this; }



