#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "IncendiaryDamageType.generated.h"

UCLASS()
class UIncendiaryDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UIncendiaryDamageType();

	void SetOnFire();
};
