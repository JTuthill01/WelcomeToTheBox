#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "InteractableContainers.generated.h"

UCLASS()
class AInteractableContainers : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AInteractableContainers();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
};
