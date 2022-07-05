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
	virtual void InitializeFor(APlayerController* PC) override;
	virtual void UpdateCamera(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;
};
