// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPLoaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBPLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPLoaderComponent();

public:
	FORCEINLINE void SetOutPathString(FString InFilePathString) { LoadString = InFilePathString; }

	UFUNCTION()
	FString LoadParser(FString WeaponString);

private:
	FString LoadString;
};
