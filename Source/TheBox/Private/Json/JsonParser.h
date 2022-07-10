#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonParser.generated.h"

UCLASS()
class AJsonParser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJsonParser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
