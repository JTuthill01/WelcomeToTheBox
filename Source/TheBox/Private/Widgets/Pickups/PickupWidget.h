#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupWidget.generated.h"

UCLASS()
class UPickupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPickupWidget(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> PickupIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PickupText;
};
