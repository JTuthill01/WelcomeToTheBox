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
		{
			UGameplayStatics::SpawnSoundAttached(PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->GetMagTapSound(), PlayerRef->GetWeaponMap()[PlayerRef->GetCurrentWeaponName()]->GetWeaponMesh());
		}
	}
}
