#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/WeaponData/Str_WeaponStats.h"
#include "TheBoxGameInstance.generated.h"

UCLASS()
class UTheBoxGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer);

	UTheBoxGameInstance();

	UFUNCTION()
	FString LoadParser(FString WeaponString);

protected:
	void Init() override;
};
