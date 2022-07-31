#include "AnimNotifies/WeaponReload/MagTap/MagTapAnimNotify.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponBase/WeaponBase.h"

void UMagTapAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& PlayerRef = Cast<APlayerCharacter>(MeshComp->GetOwner());

		if (IsValid(PlayerRef))
			UGameplayStatics::SpawnSoundAttached(PlayerRef->GetCurrentWeapon()->GetMagTapSound(), PlayerRef->GetCurrentWeapon()->GetWeaponMesh());
	}
}
