#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TheBoxCameraManager.generated.h"

UCLASS()
class ATheBoxCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ATheBoxCameraManager();

public:
	virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
