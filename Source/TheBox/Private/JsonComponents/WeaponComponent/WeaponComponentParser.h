#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/WeaponEnums/WeaponEnums.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
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

	UPROPERTY()
	FWeaponDataStats WeaponData;

private:
	FString ObjectString;
};
