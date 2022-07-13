#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "WeaponComponentParser.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponComponentParser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponentParser();

	FORCEINLINE void SetObjectData(FString InObjectString) { ObjectString = InObjectString; }

public:
	void WeaponParser();

public:
	FString MeshFilePathString;

	int32 CurrentMagTotal;

	int32 MaxMagTotal;

	int32 CurrentTotalAmmo;

	int32 MaxTotalAmmo;

	int32 LowAmmo;

	int32 CrosshairIndex;

	int32 WeaponIndex;

	float DamageAmount;

	float CriciticalHitChance;

	float DamageRadius;

	FName NameOfWeapon;

	EWeaponFireType FireType;

	UPROPERTY()
	TObjectPtr<class UTexture2D> Icon;

	UPROPERTY()
	TObjectPtr<class UNiagaraSystem> AmmoEject;

	UPROPERTY()
	TObjectPtr<class UNiagaraSystem> FireFX;

	UPROPERTY()
	TObjectPtr<class USoundBase> FireSound;

	UPROPERTY()
	TObjectPtr<class USoundBase> RackSlideSound;

	UPROPERTY()
	TObjectPtr<class USoundBase> MagOutSound;

	UPROPERTY()
	TObjectPtr<class USoundBase> MagInSound;

private:
	FString ObjectString;
};
