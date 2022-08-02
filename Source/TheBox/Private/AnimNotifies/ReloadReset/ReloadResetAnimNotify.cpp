#include "AnimNotifies/ReloadReset/ReloadResetAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"

void UReloadResetAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

		if (!IsValid(Weapon))
			return;

		Weapon->SetCanShgotgunFireOrReload(true);
	}

	else
		return;
}
