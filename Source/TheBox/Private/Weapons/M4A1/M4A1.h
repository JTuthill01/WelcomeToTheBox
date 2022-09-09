// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "M4A1.generated.h"

/**
 * 
 */
UCLASS()
class AM4A1 : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void SetData() override;
};
