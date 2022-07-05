#include "Misc/CameraManager/TheBoxCameraManager.h"
#include <Kismet/GameplayStatics.h>

ATheBoxCameraManager::ATheBoxCameraManager() = default;

void ATheBoxCameraManager::InitializeFor(APlayerController* PC)
{
	Super::InitializeFor(PC);
}

void ATheBoxCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
}

void ATheBoxCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
