#include "AnimNotifies/ShotgunRack/ShotgunRackAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UShotgunRackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto&& Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (!IsValid(Weapon))
		return;

	else 
		UGameplayStatics::SpawnSoundAttached(Weapon->WeapStats.RackSlideSound, MeshComp);
}
