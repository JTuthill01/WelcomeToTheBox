#include "Widgets/InteractableFound/InteractableFoundWidget.h"

UInteractableFoundWidget::UInteractableFoundWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UInteractableFoundWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetDesiredSizeInViewport(FVector2D(400.F, 400.F));

	ScreenSize = FVector2D(GetScreenSize().X / 2, GetScreenSize().Y / 2);

	SetPositionInViewport(ScreenSize);
}

bool UInteractableFoundWidget::Initialize()
{
	Super::Initialize();

	return true;
}

FVector2D UInteractableFoundWidget::GetScreenSize()
{
	FVector2D Result = FVector2D(0.F);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}
