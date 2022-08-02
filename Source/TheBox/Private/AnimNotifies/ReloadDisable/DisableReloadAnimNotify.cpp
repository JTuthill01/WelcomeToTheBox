#include "AnimNotifies/ReloadDisable/DisableReloadAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"

void UDisableReloadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

		if (!IsValid(Weapon))
			return;

		Weapon->SetCanShgotgunFireOrReload(false);
	}

	else
		return;
}
