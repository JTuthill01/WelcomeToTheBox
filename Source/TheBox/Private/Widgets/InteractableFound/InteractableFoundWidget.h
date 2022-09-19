#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableFoundWidget.generated.h"

UCLASS()
class UInteractableFoundWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInteractableFoundWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InteractText;

private:
	FVector2D GetScreenSize();

	FVector2D ScreenSize;
};
