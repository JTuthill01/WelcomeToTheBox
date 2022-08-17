#include "AnimNotifies/GrenadeRelease/GrenadeReleaseAnimNotify.h"
#include "Character/Player/PlayerCharacter.h"

void UGrenadeReleaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Player = Cast<APlayerCharacter>(MeshComp->GetOwner());

		if (IsValid(Player))
			Player->OnGrenadeReleased();

		else
			return;
	}

	else
		return;
}
