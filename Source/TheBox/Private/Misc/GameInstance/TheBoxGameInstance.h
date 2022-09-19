#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "Interfaces/Instance/SetGameInstnaceInterface.h"
#include "TheBoxGameInstance.generated.h"

UCLASS()
class UTheBoxGameInstance : public UGameInstance, public ISetGameInstnaceInterface
{
	GENERATED_BODY()
	
public:
	UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer);

	UTheBoxGameInstance();

	UFUNCTION()
	FString LoadParser(FString WeaponString);

	virtual UTheBoxGameInstance* SetInstance_Implementation() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UTexture2D> Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EWeaponName, TObjectPtr<class UTexture2D>> IconMap;

	UPROPERTY()
	TMap<EWeaponName, TObjectPtr<class UNiagaraSystem>> FireFXMap;

	UPROPERTY()
	TMap<EWeaponName, TObjectPtr<class UNiagaraSystem>> AmmoEjectFXMap;

protected:
	void Init() override;

private:
	void IconLoader();

	void FireFXLoader();

	void AmmoEjectFXLoader();
};
