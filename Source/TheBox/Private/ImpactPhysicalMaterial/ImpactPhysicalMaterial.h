#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Structs/ImpactFX/Str_ImpactFX.h"
#include "ImpactPhysicalMaterial.generated.h"

UCLASS()
class UImpactPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UImpactPhysicalMaterial();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FImpactEffects LineTraceImpactEffect;
};
