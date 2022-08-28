#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReloadWidget.generated.h"

UCLASS()
class UReloadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UReloadWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	FVector2D GetScreenSize();

	FVector2D ScreenSize;
};
