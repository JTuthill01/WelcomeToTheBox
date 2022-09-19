#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "KeysAndKeyCards.generated.h"

UCLASS()
class AKeysAndKeyCards : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeysAndKeyCards();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> KeyMesh;

public:
	void InteractableFound_Implementation() override;


	void InteractWithObject_Implementation() override;


	void ClearViewport_Implementation() override;

};
