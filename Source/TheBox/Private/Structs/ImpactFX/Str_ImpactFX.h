#pragma once

#include "Str_ImpactFX.generated.h"

USTRUCT(BlueprintType)
struct FImpactEffects
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFXSystemAsset* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInstance* ImpactDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector DecalSize { 2.5F, 2.5F, 2.5F };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DecalLifeTime = 8.F;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* ImpactSound;
};