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
		return FColor::FromHex(TEXT("8F3152"));
	}

	FORCEINLINE static FColor HazelGreen()
	{
		return FColor::FromHex(TEXT("617C58"));
	}

	FORCEINLINE static FColor DarkOliveGreen()
	{
		return FColor::FromHex(TEXT("556B2F"));
	}

	FORCEINLINE static FColor DarkPurple()
	{
		return FColor::FromHex(TEXT("452F6B"));
	}

    static FORCEINLINE FColor CreateUndefined(FColor& OutColor, FString InHexString)
    {
        OutColor = FColor::FromHex(InHexString);

        return OutColor;
    }
};