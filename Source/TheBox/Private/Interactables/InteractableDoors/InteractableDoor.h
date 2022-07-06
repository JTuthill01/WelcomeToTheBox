#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "InteractableDoor.generated.h"

UCLASS()
class AInteractableDoor : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AInteractableDoor();

public:
	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorMesh;
};
