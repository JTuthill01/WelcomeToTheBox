#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TheBoxGameInstance.generated.h"

UCLASS()
class UTheBoxGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer);

protected:
	void Init() override;
};
