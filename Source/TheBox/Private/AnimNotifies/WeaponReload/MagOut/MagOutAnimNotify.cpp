#include "AnimNotifies/WeaponReload/MagOut/MagOutAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UMagOutAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Current = Cast<AWeaponBase>(MeshComp->GetOwner());

		if (IsValid(Current))
			UGameplayStatics::SpawnSoundAttached(Current->WeapStats.MagOutSound, MeshComp);
	}
}
