#include "DamageTypes/IncendiaryDamageType.h"

UIncendiaryDamageType::UIncendiaryDamageType()
{
	bCausedByWorld = false;

	DamageImpulse = 0.F;

	DestructibleImpulse = 0.F;

	DestructibleDamageSpreadScale = 1.F;

	bScaleMomentumByMass = true;

	DamageFalloff = 1.F;
}
