#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Components/TimelineComponent.h"
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

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ControlDoor();

	UFUNCTION()
	void ToggleDoor();

	UFUNCTION()
	void SetState();

	UFUNCTION()
	void InitializeDoor();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FloatCurve, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> DoorCurve;

	
	FTimeline DoorTimeline;

private:
	bool bIsOpen;
	bool bIsReady;

	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;

	FRotator DoorRotation;
};
