#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/PickupData/Str_PickupData.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "PickupBase.generated.h"

UCLASS()
class APickupBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	FPickupData PickupData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TObjectPtr<UDataTable> PickupDataTable;
};
