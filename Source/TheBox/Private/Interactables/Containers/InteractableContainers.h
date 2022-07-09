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
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SubMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> DoorTimelineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FloatCurve, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> DoorCurve;

private:
	bool bIsOpen;
	bool bIsReady;

	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;

	FRotator DoorRotation;
};
