#pragma once

#include "Math/Color.h"
#include "Str_CustomColors.generated.h"

USTRUCT(BlueprintType)
struct FCustomColors
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor ForestGreen = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("228B22")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DarkGreen = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("006400")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor FireBrickRed = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("B22222")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DarkRed = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("8B0000")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor PlainRed = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FF0000")));
};
