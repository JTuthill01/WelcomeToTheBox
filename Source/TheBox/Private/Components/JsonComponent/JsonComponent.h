#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JsonComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UJsonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJsonComponent();

	UFUNCTION()
	void Parser();

	UPROPERTY()
	FString InString;
};
