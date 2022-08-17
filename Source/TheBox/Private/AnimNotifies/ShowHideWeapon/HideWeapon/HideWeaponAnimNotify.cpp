#include "AnimNotifies/ShowHideWeapon/HideWeapon/HideWeaponAnimNotify.h"
#include "Character/Player/PlayerCharacter.h"

void UHideWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Player = Cast<APlayerCharacter>(MeshComp->GetOwner());

		if (IsValid(Player))
			Player->SetWeaponVisibility(true);
	}
}
