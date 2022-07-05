#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Interfaces/Controller/PlayerControllerInterface.h"
#include "PlayerCharacterController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class APlayerCharacterController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()
	
public:
	APlayerCharacterController();

public:
	virtual void SetupInputComponent() override;

	virtual APlayerCharacterController* SetControllerRef_Implementation() override;
protected:
	virtual void BeginPlay() override;

protected:

#pragma region INPUT

	//Input Actions//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* InteractAction;

	//Mapping Contexts//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	int32 BaseMappingPriority = 0;

	//Input Functions//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void Jump();
	void StopJump();

#pragma endregion

private:
	UPROPERTY();
	TObjectPtr<class APlayerCharacter> PlayerRef;
};
