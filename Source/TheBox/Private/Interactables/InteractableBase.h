#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "InteractableBase.generated.h"

UCLASS()
class AInteractableBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

public:
	UFUNCTION()
	void OnClearViewport();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InteractableFound_Implementation() override;

	virtual void InteractWithObject_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseRoot)
	TObjectPtr<class USceneComponent> BaseRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widgets)
	TSubclassOf<class UUserWidget> InteractableWidgetSub;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widgets)
	TObjectPtr<class UTexture2D> IconTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widgets)
	FText WidgetText;

	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<class UInteractableFoundWidget> InteractableFoundWidget;
};
