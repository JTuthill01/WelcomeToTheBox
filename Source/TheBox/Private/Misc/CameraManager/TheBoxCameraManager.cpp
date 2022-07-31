#include "Misc/CameraManager/TheBoxCameraManager.h"

ATheBoxCameraManager::ATheBoxCameraManager() = default;

void ATheBoxCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void ATheBoxCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);
}


