#pragma once

#include "Math/Color.h"
#include "Str_CustomHexColors.generated.h"

USTRUCT(BlueprintType)
struct FCustomColorsFromHex
{
    GENERATED_BODY()

    static FORCEINLINE FColor NightBlue()
    {
        return FColor::FromHex(TEXT("151B54"));
    }

    static FORCEINLINE FColor DeepTeal()
    {
        return FColor::FromHex(TEXT("033E3E"));
    }

    static FORCEINLINE FColor DeepSeaGreen()
    {
        return FColor::FromHex(TEXT("306754"));
    }

    FORCEINLINE static FColor Mint()
    {
        return FColor::FromHex(TEXT("31906E"));
    }

	FORCEINLINE static FColor FireBrick()
	{
		return FColor::FromHex(TEXT("#8F3152"));
	}

	FORCEINLINE static FColor HazelGreen()
	{
		return FColor::FromHex(TEXT("#617C58"));
	}

	FORCEINLINE static FColor DarkOliveGreen()
	{
		return FColor::FromHex(TEXT("#556B2F"));
	}

	FORCEINLINE static FColor DarkForestGreen()
	{
		return FColor::FromHex(TEXT("#254117"));
	}

	FORCEINLINE static FColor ShamrockGreen()
	{
		return FColor::FromHex(TEXT("#347C17"));
	}

	FORCEINLINE static FColor NeonGreen()
	{
		return FColor::FromHex(TEXT("#16F529"));
	}

	FORCEINLINE static FColor NeonYellow()
	{
		return FColor::FromHex(TEXT("#FFFF33"));
	}

	FORCEINLINE static FColor NeonOrange()
	{
		return FColor::FromHex(TEXT("#FF6700"));
	}

	FORCEINLINE static FColor NeonPurple()
	{
		return FColor::FromHex(TEXT("#9D00FF"));
	}

	FORCEINLINE static FColor DarkPurple()
	{
		return FColor::FromHex(TEXT("#452F6B"));
	}

	FORCEINLINE static FColor RubberDuckyYellow()
	{
		return FColor::FromHex(TEXT("#FFD801"));
	}

	FORCEINLINE static FColor OrangeRed()
	{
		return FColor::FromHex(TEXT("#FF4500"));
	}

	FORCEINLINE static FColor RedWine()
	{
		return FColor::FromHex(TEXT("#990012"));
	}

	FORCEINLINE static FColor DarkRed()
	{
		return FColor::FromHex(TEXT("#8B0000"));
	}

	FORCEINLINE static FColor BloodNight()
	{
		return FColor::FromHex(TEXT("#551606"));
	}

	FORCEINLINE static FColor DeepPurple()
	{
		return FColor::FromHex(TEXT("#36013F"));
	}

	FORCEINLINE static FColor PurpleMonster()
	{
		return FColor::FromHex(TEXT("#461B7E"));
	}

	FORCEINLINE static FColor RichLilac()
	{
		return FColor::FromHex(TEXT("#B666D2"));
	}

	FORCEINLINE static FColor TronBlue()
	{
		return FColor::FromHex(TEXT("#7DFDFE"));
	}

	FORCEINLINE static FColor NeonBlue()
	{
		return FColor::FromHex(TEXT("#1589FF"));
	}

	FORCEINLINE static FColor MediumAquaMarine()
	{
		return FColor::FromHex(TEXT("#66CDAA"));
	}

	FORCEINLINE static FColor MagicMint()
	{
		return FColor::FromHex(TEXT("#AAF0D1"));
	}

	FORCEINLINE static FColor DullSeaGreen()
	{
		return FColor::FromHex(TEXT("#4E8975"));
	}

	FORCEINLINE static FColor Jade()
	{
		return FColor(0, 163, 108, 255);
	}

	FORCEINLINE static FColor EarthGreen()
	{
		return FColor::FromHex(TEXT("#34A56F"));
	}

	FORCEINLINE static FColor DarkOrange()
	{
		return FColor::FromHex(TEXT("#FF8C00"));
	}

	FORCEINLINE static FColor IndianSaffron()
	{
		return FColor::FromHex(TEXT("#FF7722"));
	}

	FORCEINLINE static FColor SaffronRed()
	{
		return FColor::FromHex(TEXT("#931314"));
	}

	FORCEINLINE static FColor PurpleLily()
	{
		return FColor::FromHex(TEXT("#550A35"));
	}

	FORCEINLINE static FColor PlumVelvet()
	{
		return FColor::FromHex(TEXT("#7D0552"));
	}

	FORCEINLINE static FColor Snow()
	{
		return FColor::FromHex(TEXT("#FFFAFA"));
	}

    FORCEINLINE static FColor CreateUndefined(FColor& OutColor, FString InHexString)
    {
        OutColor = FColor::FromHex(InHexString);

        return OutColor;
    }
};