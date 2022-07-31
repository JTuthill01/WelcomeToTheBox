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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Platinum = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("E5E4E2")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GrayGoose = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("D1D0CE")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor JetGray = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("616D7E")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor LightWhite = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FFFFF7")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Gainsboro = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#dcdcdc")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GainsboroDark = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#525252")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor BlackLeatherJacket = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#253529")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Onyx = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#353839")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor KombuGreen = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#354230")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Charcoal = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#34282C")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Oil = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#3B3131")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DarkGray = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#3A3B3C")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Night = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#0C090A")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Licorice = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#1B1212")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Snow = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#FFFAFA")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GhostWhite = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#F8F8FF")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor MilkWhite = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#FEFCFF")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Cotton = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#FBFBF9")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor WhiteSmoke = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#F5F5F5")));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor CustomWhite = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#FFFFFF")));
};
