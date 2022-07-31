#include "AnimNotifies/WeaponReload/SlideRack/SlideRackAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void USlideRackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Current = Cast<AWeaponBase>(MeshComp->GetOwner());

		if (IsValid(Current))
			UGameplayStatics::SpawnSoundAttached(Current->WeapStats.MagInSound, MeshComp);
	}
}
