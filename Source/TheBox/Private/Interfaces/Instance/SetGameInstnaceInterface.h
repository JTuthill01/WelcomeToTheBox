#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SetGameInstnaceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USetGameInstnaceInterface : public UInterface
{
	GENERATED_BODY()
};

class ISetGameInstnaceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class UTheBoxGameInstance* SetInstance(); 
};
